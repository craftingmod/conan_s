package net.tarks.jh;

import javafx.application.Application;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class Main {

    private static final int MODE_TOGGLE8 = 1;
    private static final int MODE_TOGGLE16 = 5;
    private static final int MODE_READ = 6;
    private static final int MODE_UTF8 = 2;
    private static final int MODE_UTF16 = 3;
    private static final int MODE_EUCKR = 4;

    public static File rootFile = null;
    public static String extensions = null;
    public static Boolean exclusive_mode = null;

    public static void main(String[] args) {
        boolean help = false;
        //String[] sub = {"-ex","c,h","-r","/Users/superuser/Downloads/conan_s-develop/conan","develop/Conan"};
        int mode = 0;
        boolean req_ext = false;
        ArrayList<String> dirName = new ArrayList<>();
        File customDir = null;
        for (String s : args) {
            if (s.equalsIgnoreCase("-help")) {
                help = true;
            } else if (s.equalsIgnoreCase("-toggle8")) {
                mode = MODE_TOGGLE8;
            } else if (s.equalsIgnoreCase("-toggle16")) {
                mode = MODE_TOGGLE16;
            } else if (s.equalsIgnoreCase("-utf8")) {
                mode = MODE_UTF8;
            } else if (s.equalsIgnoreCase("-utf16")) {
                mode = MODE_UTF16;
            } else if (s.equalsIgnoreCase("-euckr")) {
                mode = MODE_EUCKR;
            } else if (s.equalsIgnoreCase("-list")) {
                mode = MODE_READ;
            } else if (s.equalsIgnoreCase("-r")) {
                exclusive_mode = Boolean.TRUE;
            } else if (s.equalsIgnoreCase("-ex")) {
                req_ext = true;
            } else if (req_ext) {
                req_ext = false;
                extensions = s.replaceAll("\\.","");
            } else {
                dirName.add(s);
            }
        }
        /**
         * 1. Help
         */
        if(help){
            trace("UTF-8과 EUC-KR을 서로 전체 변환 해주는 유틸입니다.");
            trace("~.jar [옵션] [폴더]\n");
            trace("* 옵션");
            trace("-----------------------------------------------");
            trace(" * 아래 옵션들은 GUI창을 열지 않습니다.");
            trace(" -toggle8: 자동으로 UTF-8과 EUC-KR 사이를 전환해줍니다.");
            trace(" -toggle16: 자동으로 UTF-16과 EUC-KR 사이를 전환해줍니다.");
            trace(" -utf8: UTF-8로 모두 변환합니다.");
            trace(" -utf16: UTF-16(LE+BOM)로 모두 변환합니다.");
            trace(" -euckr: EUC-KR로 모두 변환합니다.");
            trace(" -list: 목록을 보여주기만 합니다.");
            trace("-----------------------------------------------");
            trace(" * 아래 옵션들은 GUI창 여부에 영향을 주지 않습니다.");
            trace(" -r: 하위 디렉토리를 모두 검사하여 적용합니다. (CUI만)");
            trace(" -ex <확장자> (c,cpp,h): 변환할 파일들을 재정의 합니다.");
            trace(" \"[폴더]\": 변환할 폴더를 강제로 변경합니다.");
            trace("-----------------------------------------------");
            return;
        }
        /**
         * 1.5? Say option
         */
        if(exclusive_mode != null && exclusive_mode.booleanValue()){
            trace("Exclusive mode");
        }
        if(extensions != null){
            trace("Custom extensions: " + extensions);
        }
        /**
         * 2. Directory
         */
        // custom folder
        if(dirName.size() >= 1){
            StringBuilder path = new StringBuilder();
            for(int i=0;i<dirName.size();i+=1){
                String s = dirName.get(i);
                if(i == 0){
                    if(s.length() >= 1 && (s.startsWith("\"") || s.startsWith("'"))){
                        s = s.substring(1);
                    }
                }
                if(i == dirName.size()-1){
                    if(s.length() >= 1 && (s.endsWith("\"") || s.endsWith("'"))){
                        s = s.substring(0,s.length()-1);
                    }
                }
                path.append(s);
                if(i < dirName.size()-1){
                    path.append(" ");
                }
            }
            customDir = new File(path.toString());
            if(!customDir.exists() || !customDir.isDirectory()){
                System.out.printf("\"%s\" 폴더가 존재하지 않습니다!\n",customDir.getAbsolutePath());
                return;
            }else{
                System.out.printf("Custom Directory: \"%s\"\n",customDir.getAbsolutePath());
            }
            rootFile = customDir;
        }else{
            System.out.printf("Default Directory: \"%s\"\n",UniConv.getLaunchDir().getAbsolutePath());
        }
        /**
         * 3. CUI?
         */
        trace("-----------------------------------------------");
        if(mode >= 1){
            ThreadBlock threadBlock = new ThreadBlock();
            threadBlock.start();
            boolean exclusive = exclusive_mode != null && exclusive_mode;
            Thread threadWorker = new ThreadWork(threadBlock,mode,exclusive);
            threadWorker.start();
            synchronized (threadBlock) {
                try{
                    threadBlock.wait();
                }catch (InterruptedException e){
                    e.printStackTrace();
                }
            }
            return;
        }
        /**
         * GUI Mode!
         */
        Application.launch(MainActivity.class,args);
    }
    public static void trace(String s){
        System.out.println(s);
    }
    public static String readFile(String path, Charset encoding)
            throws IOException
    {
        byte[] encoded = Files.readAllBytes(Paths.get(path));
        return new String(encoded, encoding);
    }
    public static class ThreadWork extends Thread implements Runnable, UniConv.onCompleteListener {
        private ThreadBlock _tb;
        private int _mode;
        private boolean _exclusive;
        private String _enc;
        private UniConv converter;
        private int _PROGRESS = -53;
        public ThreadWork(ThreadBlock tb,int mode,boolean ex){
            this._tb = tb;
            this._exclusive = ex;
            this._mode = mode;
            this._exclusive = ex;
            this._enc = null;
        }
        @Override
        public void run() {
            converter = new UniConv(rootFile == null ? UniConv.getLaunchDir() : rootFile);
            if(extensions != null){
                converter.defineExt(extensions.split(","));
            }
            converter.setExclusive(this._exclusive);
            switch (this._mode){
                case MODE_UTF8:
                    this._enc = "UTF-8";
                    break;
                case MODE_UTF16:
                    this._enc = "UnicodeLittle";
                    break;
                case MODE_EUCKR:
                    this._enc = "EUC-KR";
                    break;
            }
            if(this._mode == MODE_TOGGLE8 ||this._mode == MODE_TOGGLE16 || this._mode == MODE_READ){
                converter.read(this);
            }else{
                converter.convert(this._enc,this);
            }
        }

        @Override
        public void onComplete(int type, Object data) {
            if(type == UniConv.COM_READ_ALL){
                ArrayList<FData> ecs = (ArrayList<FData>) data;
                int utf8 = 0;
                int utf16 = 0;
                int euckr = 0;
                for(FData fd : ecs){
                    String e = fd.text.size() >= 1 ? fd.text.get(0) : "";
                    if(this._mode == MODE_READ){
                        StringBuilder tab = new StringBuilder();
                        for(int i=e.length()/4;i<4;i+=1){
                            tab.append("\t");
                        }
                        System.out.printf(" %s%s ->  \"%s\"\n",e,tab.toString(),
                                fd.file.getAbsolutePath().replace(converter.getRootDir().getAbsolutePath(),"."));
                        continue;
                    }
                    if(e.equalsIgnoreCase("UTF-8")){
                        utf8 += 1;
                    }else if(e.startsWith("UTF-16")){
                        utf16 += 1;
                    }else if(e.equalsIgnoreCase("EUC-KR")){
                        euckr += 1;
                    }
                }
                if(utf8+utf16 > euckr){
                    this._enc = "EUC-KR";
                }else{
                    if(this._mode == MODE_TOGGLE8){
                        this._enc = "UTF-8";
                    }else if(this._mode == MODE_TOGGLE16){
                        this._enc = "UnicodeLittle";
                    }
                }
                if(this._mode == MODE_READ){
                    // Finish!
                    System.out.println("\n\nComplete!");
                    this._tb.finish = true;
                }else{
                    converter.convert(this._enc,this);
                }
            }else if(type == UniConv.COM_WRITE_ALL){
                System.out.println("\n\nComplete!");
                this._tb.finish = true;
            }else if(type == UniConv.COM_ERROR){
                System.err.println("\n\nError!");
                this._tb.finish = true;
            }
        }
    }
    public static class ThreadBlock extends Thread {
        boolean finish;
        public ThreadBlock(){
            finish = false;
        }
        @Override
        public void run() {
            synchronized (this) {
                for(int i=0;i<6000;i+=1){
                    if(finish){
                        break;
                    }
                    try{
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                notify();
            }
        }
    }
}
