package net.tarks.jh;

import org.mozilla.intl.chardet.nsDetector;
import org.mozilla.intl.chardet.nsICharsetDetectionObserver;
import org.mozilla.intl.chardet.nsPSMDetector;
import org.mozilla.universalchardet.UniversalDetector;

import java.io.*;
import java.net.URLDecoder;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.NotDirectoryException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Conv {
    private static final String[] whitelist = {"c","cpp","h","txt"};
    public static final int UTF8 = 1;
    public static final int CP949 = 2;
    public static final int EUCKR = 3;
    public static final int COM_ERROR = -1;
    public static final int COM_FILE_CONTENT = 10;
    public static final int COM_FILE_ENCODING = 11;
    public static final int COM_CONVERT = 22;
    public static final File getLaunchDir(){
        String path = Conv.class.getProtectionDomain().getCodeSource().getLocation().getPath();
        String decodedPath = "./";
        try {
            decodedPath = URLDecoder.decode(path, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        if(decodedPath.endsWith(".jar")){
            decodedPath = decodedPath.substring(0,Math.max(decodedPath.lastIndexOf("\\"),decodedPath.lastIndexOf("/"))+1);
        }
        return new File(decodedPath);
    }
    private File rootDir;
    private ArrayList<FData> convFiles;
    private ArrayList<File> loadFiles;
    private int que;
    public Conv(File dir){
        rootDir = dir;
        convFiles = new ArrayList<>();
    }
    public void convert(final onCompleteListener listener){
        loadFiles = getAllConvFiles(rootDir,false);
        System.out.println("loadFiles length: " + loadFiles.size());
        if(loadFiles.size() == 0){
            listener.onComplete(COM_CONVERT,null);
        }
        que = 0;
        convFiles.clear();
        try {
            getFileText(loadFiles.get(que), new onCompleteListener() {
                @Override
                public void onComplete(int type, Object data) {
                    if(type == COM_FILE_CONTENT){
                        convFiles.add((FData) data);
                    }else if(type == COM_ERROR){
                        System.err.println("Read Error at " + que);
                    }
                    if(que >= 0){
                        que += 1;
                        if(que < loadFiles.size()){
                            try {
                                getFileText(loadFiles.get(que),this);
                            } catch (IOException e) {
                                this.onComplete(COM_ERROR,null);
                            }
                        }else{
                            System.out.println("Finish!");
                            que = -1;
                        }
                    }
                }
            });
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void getFileText(final File f,final onCompleteListener listener) throws IOException {
        if(f == null || !f.exists() || f.isDirectory()){
            throw new IOException("There's no file in " + f.getAbsolutePath());
        }
        onCompleteListener internalL = (type, data) -> {
            if(type == COM_FILE_ENCODING){
                String encoding = (String) data;
                System.out.println(f.getAbsolutePath() +"'s Encoding: " + encoding);
                if(encoding.contains("#")){
                    encoding = encoding.substring(0,encoding.indexOf("#"));
                }
                try {
                    String text = new String(Files.readAllBytes(Paths.get(f.toURI())), Charset.forName(encoding));
                    if(encoding.equals("windows-1252")){
                        // detect again
                        encoding = "UTF-16";
                        //System.out.println("RR: " + new String(Files.readAllBytes(Paths.get(f.toURI())),encoding));
                    }
                    listener.onComplete(COM_FILE_CONTENT,new FData(f,text));
                } catch (IOException e) {
                    e.printStackTrace();
                    listener.onComplete(COM_ERROR,null);
                }
            }else if(type == COM_ERROR){
                listener.onComplete(COM_ERROR,null);
            }
        };

        nsDetector det = new nsDetector(nsPSMDetector.KOREAN);

        FileInputStream fs = new FileInputStream(f);
        byte[] buf = new byte[1024];
        int len;
        boolean done = false;
        boolean isAscii = true;
        final int buflength = buf.length;

        det.Init(s -> {
            try {
                fs.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            UniversalDetector detector = new UniversalDetector(null);
            try {
                FileInputStream fs_again = new FileInputStream(f);
                int nread;
                byte[] buf_again = new byte[1024];
                while((nread = fs_again.read(buf_again)) != -1 && !detector.isDone()) {
                    detector.handleData(buf_again, 0, nread);
                }
                fs_again.close();
                detector.dataEnd();

                String encoding = detector.getDetectedCharset();
                if(encoding != null){
                    s = s + "#" + encoding;
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }

            internalL.onComplete(COM_FILE_ENCODING,s);
        });
        while((len = fs.read(buf,0,buflength)) != -1){
            if(isAscii){
                isAscii = det.isAscii(buf,len);
            }
            if(!isAscii && !done){
                done = det.DoIt(buf,len,false);
            }
        }
        det.DataEnd();
        try{
            fs.close();
        }catch (IOException e){
            e.printStackTrace();
        }

        if(isAscii){
            internalL.onComplete(COM_FILE_ENCODING,"US-ASCII");
        }else{
            internalL.onComplete(COM_ERROR,null);
        }
    }
    private ArrayList<File> getAllConvFiles(File dir, boolean exclusive){
        ArrayList<File> outputs = new ArrayList<>();
        try {
            outputs = getAllConvFiles_I(outputs,dir,exclusive);
        } catch (NotDirectoryException e) {
            e.printStackTrace();
        }
        return outputs;
    }
    private ArrayList<File> getAllConvFiles_I(ArrayList<File> loop_file,File dir,boolean exclusive) throws NotDirectoryException {
        if(!dir.exists() || !dir.isDirectory()){
            throw new NotDirectoryException(dir.getAbsolutePath() + " is not a directory!");
        }
        if(exclusive){
            File[] dirs = dir.listFiles(pathF -> pathF.isDirectory());
            for(File loop_dir : dirs){
                loop_file = getAllConvFiles_I(loop_file,loop_dir,exclusive);
            }
        }
        File[] files_arr = dir.listFiles(pathname -> {
            String fn = pathname.getName();
            fn = fn.contains(".")?fn.substring(fn.lastIndexOf(".")+1):"";
            if(pathname.isDirectory()){
                return false;
            }
            for(String p : whitelist){
                if(p.equals(fn)){
                    return true;
                }
            }
            return false;
        });
        loop_file.addAll(Arrays.asList(files_arr));
        return loop_file;
    }
    public interface onCompleteListener {
        void onComplete(int type,Object data);
    }
}
