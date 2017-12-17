package net.tarks.jh;

import org.mozilla.intl.chardet.nsDetector;
import org.mozilla.intl.chardet.nsICharsetDetectionObserver;
import org.mozilla.intl.chardet.nsPSMDetector;

import java.io.*;
import java.net.URLDecoder;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.NotDirectoryException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Conv {
    private static final String[] whitelist = {"c","cpp","h","txt"};
    public static final int UTF8 = 1;
    public static final int CP949 = 2;
    public static final int EUCKR = 3;
    public static final int COM_FILE_CONTENT = 10;
    public static final int COM_FILE_ENCODING = 11;
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
    public Conv(File dir){
        rootDir = dir;
        try {
            ArrayList<File> files = getAllConvFiles(rootDir,true);
            for(File f:files){
                getFileDesc(f, (type, data) -> {

                });
            }
        } catch (NotDirectoryException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void getFileDesc(final File f,final onCompleteListener listener) throws IOException {
        if(f == null || !f.exists() || f.isDirectory()){
            throw new IOException("There's no file in " + f.getAbsolutePath());
        }
        int lang = nsPSMDetector.KOREAN;
        onCompleteListener internalL = (type, data) -> {
            if(type == COM_FILE_ENCODING){
                String encoding = (String) data;
                System.out.println(f.getAbsolutePath() +"'s Encoding: " + encoding);
            }
        };

        nsDetector det = new nsDetector(lang);
        det.Init(s -> internalL.onComplete(COM_FILE_ENCODING,s));

        FileInputStream fs = new FileInputStream(f);
        byte[] buf = new byte[1024];
        int len;
        boolean done = false;
        boolean isAscii = true;
        final int buflength = buf.length;
        while((len = fs.read(buf,0,buflength)) != -1){
            if(isAscii){
                isAscii = det.isAscii(buf,len);
            }
            if(!isAscii && !done){
                done = det.DoIt(buf,len,false);
            }
        }
        det.DataEnd();

        if(isAscii){
            internalL.onComplete(COM_FILE_ENCODING,"ascii");
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
