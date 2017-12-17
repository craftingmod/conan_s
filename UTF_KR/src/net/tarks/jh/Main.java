package net.tarks.jh;

import javafx.application.Application;

import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Main {

    public static void main(String[] args) {
        try {
            System.out.println(readFile("/Users/superuser/Downloads/conan_s-develop/conan_s-develop/Conan/appinfo.h",Charset.forName("UTF-8")));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Application.launch(MainActivity.class,args);
    }
    public static String readFile(String path, Charset encoding)
            throws IOException
    {
        byte[] encoded = Files.readAllBytes(Paths.get(path));
        return new String(encoded, encoding);
    }
}
