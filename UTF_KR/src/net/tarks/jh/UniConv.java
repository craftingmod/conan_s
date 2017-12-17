package net.tarks.jh;

import org.mozilla.universalchardet.UniversalDetector;

import java.io.*;
import java.net.URLDecoder;
import java.nio.charset.Charset;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class UniConv {
    private static final String[] whitelist = {"c","cpp","h","txt"};
    public static final int UTF8 = 1;
    public static final int CP949 = 2;
    public static final int EUCKR = 3;
    public static final int COM_ERROR = -1;
    public static final int COM_ERROR_IGNORE = -2;
    public static final int COM_FILE_CONTENT = 10;
    public static final int COM_FILE_ENCODING = 11;
    public static final int COM_READ_ALL = 12;
    public static final int COM_WRITE_ALL = 14;
    public static final int COM_CONVERT = 22;
    public static final File getLaunchDir(){
        String path = UniConv.class.getProtectionDomain().getCodeSource().getLocation().getPath();
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
    private onCompleteListener errorHandler;
    private ArrayList<FData> convFiles;
    private ArrayList<File> loadFiles;
    private int que;
    public UniConv(File dir){
        rootDir = dir;
        errorHandler = null;
        convFiles = new ArrayList<>();
    }
    public File getRootDir(){
        return rootDir;
    }
    public void handleError(final onCompleteListener eL){
        errorHandler = eL;
    }
    public void convert(final String charsetN,final onCompleteListener listener){
        convert(charsetN, listener,true,false);
    }
    public void convert(final String charsetN,final onCompleteListener listener,boolean encode,boolean exclusive){
        convFiles.clear();
        que = 0;

        final ArrayList<FData> encodings = new ArrayList<>();

        loadFiles = getAllConvFiles(rootDir,exclusive);
        System.out.println("loadFiles length: " + loadFiles.size());
        if(loadFiles.size() == 0){
            listener.onComplete(COM_CONVERT,null);
        }
        onCompleteListener encode_I = (type, data) -> {
            if(type == COM_ERROR) {
                listener.onComplete(COM_ERROR, "ERROR!");
            }else if(!encode){
                listener.onComplete(COM_READ_ALL, encodings);
            }else if(type == COM_READ_ALL){
                for(FData fdata : convFiles){
                    try{
                        if(!fdata.file.canWrite()){
                            throw new IOException("Can't write \"" + fdata.file.getAbsolutePath() + "\"");
                        }else{
                            System.out.println("Write path: " + Paths.get(fdata.file.toURI()).toString() + " / Charset: " + charsetN);
                            write(fdata.file,fdata.text,charsetN);
                            //Files.write(Paths.get(fdata.file.toURI()),fdata.text,Charset.forName(charsetN), StandardOpenOption.SYNC,StandardOpenOption.WRITE);
                        }
                    }catch (IOException e){
                        e.printStackTrace();
                    }
                }
                listener.onComplete(COM_WRITE_ALL, null);
            }
        };
        try {
            getFileText(loadFiles.get(que), new onCompleteListener() {
                @Override
                public void onComplete(int type, Object data) {
                    if(type == COM_FILE_CONTENT){
                        convFiles.add((FData) data);
                    }else if(type == COM_ERROR){
                        System.err.println("Read Error at " + que);
                        if(errorHandler != null){
                            errorHandler.onComplete(COM_ERROR,"Error (log for detail)");
                        }
                        encode_I.onComplete(COM_ERROR,null);
                    }else if(type == COM_FILE_ENCODING){
                        encodings.add((FData) data);
                    }
                    if(que >= 0){
                        que += 1;
                        if(que < loadFiles.size()){
                            try {
                                getFileText(loadFiles.get(que),this, encode);
                            } catch (IOException e) {
                                this.onComplete(COM_ERROR,null);
                            }
                        }else{
                            System.out.println("Read finish!");
                            encode_I.onComplete(COM_READ_ALL,null);
                            que = -1;
                        }
                    }
                }
            }, encode);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void getFileText(final File f,final onCompleteListener listener,boolean encode) throws IOException {
        if(f == null || !f.exists() || f.isDirectory()){
            throw new IOException("There's no file in " + f.getAbsolutePath());
        }
        onCompleteListener internalL = (type, data) -> {
            if(type == COM_FILE_ENCODING){
                String encoding = (String) data;
                System.out.println(f.getAbsolutePath() +"'s Encoding: " + encoding);
                try {
                    if(encoding.equalsIgnoreCase("windows-1252")){
                        // can't detect - utf-16 LE without BOM or utf-16 BE without BOM... or nothing
                        List<String> readAs16LE = Arrays.asList(new String(Files.readAllBytes(Paths.get(f.toURI())),Charset.forName("UTF-16LE")).split("\n"));
                        List<String> readAs16BE = Arrays.asList(new String(Files.readAllBytes(Paths.get(f.toURI())),Charset.forName("UTF-16BE")).split("\n"));
                        System.err.println("Can't detect encoding of \"" + f.getAbsolutePath() + "\"");
                        boolean LE = readAs16LE.size() > readAs16BE.size();
                        System.err.println("Force. (Maybe using UTF-16 " + (LE?"LE":"BE") + ")");
                        //System.err.println(readAs16BE.split("\n").length + " / " + readAs16LE.split("\n").length);
                        //listener.onComplete(COM_ERROR_IGNORE,null);
                        if(encode){
                            listener.onComplete(COM_FILE_CONTENT,new FData(f,LE?readAs16LE:readAs16BE));
                        }else{
                            ArrayList<String> ar = new ArrayList<>();
                            ar.add(LE?"UTF-16LE":"UTF-16BE");
                            listener.onComplete(COM_FILE_ENCODING,new FData(f,ar));
                        }
                    }else{
                        List<String> text = Arrays.asList(new String(Files.readAllBytes(Paths.get(f.toURI())),Charset.forName(encoding)).split("\n"));
                        if(encode){
                            listener.onComplete(COM_FILE_CONTENT,new FData(f,text));
                        }else{
                            ArrayList<String> ar = new ArrayList<>();
                            ar.add(encoding);
                            listener.onComplete(COM_FILE_ENCODING,new FData(f,ar));
                        }
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    listener.onComplete(COM_ERROR,null);
                }
            }else if(type == COM_ERROR){
                listener.onComplete(COM_ERROR,null);
            }
        };
        UniversalDetector detector = new UniversalDetector(null);
        FileInputStream fs = new FileInputStream(f);
        byte[] buf = new byte[1024];
        int len;
        final int buflength = buf.length;

        while((len = fs.read(buf,0,buflength)) != -1 && !detector.isDone()){
            detector.handleData(buf,0,len);
        }
        detector.dataEnd();
        fs.close();

        String encoding = detector.getDetectedCharset();

        if(encoding == null){
            internalL.onComplete(COM_FILE_ENCODING,"US-ASCII");
        }else{
            internalL.onComplete(COM_FILE_ENCODING,encoding);
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
    public void write(File file, List<String> content,String charset){
        Writer out;
        if(!file.canWrite()){
            return;
        }
        try {
            out = new BufferedWriter(new OutputStreamWriter(
                    new FileOutputStream(file), charset));
            int s = content.size();
            for(int i=0;i<s;i+=1){
                out.write(content.get(i).replace("\r",""));
                if(i < s-1){
                    out.write("\n");
                }
            }
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public interface onCompleteListener {
        void onComplete(int type,Object data);
    }
}