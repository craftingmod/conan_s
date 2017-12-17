package net.tarks.jh;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class FData {
    public File file;
    public ArrayList<String> text;
    public FData(File f,List<String> t){
        file = f;
        text = new ArrayList<>();
        text.addAll(t);
    }
}
