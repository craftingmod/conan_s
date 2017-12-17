package net.tarks.jh;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.prefs.Preferences;

public class MainActivity extends Application {

    private Scene scene;
    private Parent root;
    private MainController controller;
    private Preferences pref;
    private String name_encode;
    private UniConv c;
    private ToggleGroup group;
    private String style;
    private boolean exclusive;

    @Override
    public void start(Stage stage) {
        c = new UniConv(UniConv.getLaunchDir());
        if(Main.rootFile != null){
            c.setRootDir(Main.rootFile);
        }
        if(Main.extensions != null){
            c.defineExt(Main.extensions.split(","));
        }
        FXMLLoader loader = new FXMLLoader(getClass().getResource("ui.fxml"));

        root = null;
        name_encode = "";
        try {
            root = loader.load();
        } catch (IOException e) {
            e.printStackTrace();
        }
        stage.setTitle("UniConv");
        stage.setResizable(false);
        stage.setAlwaysOnTop(false);
        stage.setIconified(false);
        scene = new Scene(root, 232, 128);

        controller = loader.getController();
        pref = Preferences.userNodeForPackage(this.getClass());
        /**
         * Insert code
         */
        controller.btn.setPrefWidth(100);
        style = controller.mainContent.getStylesheets().get(0);

        group = new ToggleGroup();
        controller.swt_utf8.setToggleGroup(group);
        controller.swt_utf16.setToggleGroup(group);
        controller.swt_euckr.setToggleGroup(group);
        controller.swt_show.setToggleGroup(group);

        exclusive = pref.getBoolean("exclusive",false);
        if(Main.exclusive_mode != null){
            this.exclusive = Main.exclusive_mode;
        }
        controller.chk_ex.setSelected(exclusive);
        controller.chk_ex.selectedProperty().addListener((o, ov, newValue) -> {
            exclusive = newValue;
            pref.putBoolean("exclusive",newValue);
        });

        controller.btn.setOnMouseClicked(event -> {
            boolean e = true;
            String encode = "UTF-8";
            if(group.getSelectedToggle().equals(controller.swt_utf8)){
                encode = "UTF-8";
            }else if(group.getSelectedToggle().equals(controller.swt_utf16)){
                encode = "UnicodeLittle";
            }else if(group.getSelectedToggle().equals(controller.swt_euckr)){
                encode = "EUC-KR";
            }else if(group.getSelectedToggle().equals(controller.swt_show)){
                e = false;
            }
            name_encode = encode;
            controller.btn.setVisible(false);
            c.convert(encode, (type, data) -> {
                controller.btn.setVisible(true);
                if(type == UniConv.COM_READ_ALL){
                    StringBuilder sb = new StringBuilder();
                    ArrayList<FData> ecs = (ArrayList<FData>) data;
                    for(FData fd : ecs){
                        String enc = fd.text.size() >= 1 ? fd.text.get(0) : "Null";
                        StringBuilder tab = new StringBuilder();
                        for(int i=enc.length()/5;i<3;i+=1){
                            tab.append("\t");
                        }
                        /*
                                                StringBuilder tab = new StringBuilder();
                        for(int i=e.length()/4;i<4;i+=1){
                            tab.append("\t");
                        }
                        System.out.printf(" %s%s ->  \"%s\"\n",e,tab.toString(),
                                fd.file.getAbsolutePath().replace(converter.getRootDir().getAbsolutePath(),"."));
                         */
                        sb.append(enc).append(tab.toString()).append(" ->  \"");
                        sb.append(fd.file.getAbsolutePath().replace(c.getRootDir().getAbsolutePath(),"."));
                        sb.append("\"\n");
                    }
                    StringBuilder sb_subT = new StringBuilder();
                    sb_subT.append("Directory: ").append(c.getRootDir().getAbsolutePath()).append("\n");
                    showFullDialog(Alert.AlertType.INFORMATION,"Encoding Info", "Encoding info of files",
                            sb_subT.toString(),"",sb.toString());
                } else if(type == UniConv.COM_WRITE_ALL){
                    Alert alert = new Alert(Alert.AlertType.INFORMATION);
                    alert.setTitle("Complete!");
                    alert.setHeaderText(null);
                    alert.setContentText("Convert to " + name_encode + " is complete.");
                    alert.getDialogPane().getStylesheets().add(style);
                    alert.show();
                }else if(type == UniConv.COM_ERROR){
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setTitle("Error!");
                    alert.setHeaderText(null);
                    alert.setContentText("Please use command line for detail.");
                    alert.getDialogPane().getStylesheets().add(style);
                    alert.show();
                }
            },e,exclusive);
        });
        c.convert("UTF-8", (type, data) -> {
            if(type == UniConv.COM_READ_ALL){
                ArrayList<FData> ecs = (ArrayList<FData>) data;
                boolean utf8 = false;
                boolean utf16 = false;
                boolean euckr = false;
                for(FData fd : ecs){
                    String e = fd.text.size() >= 1 ? fd.text.get(0) : "";
                    if(e.equalsIgnoreCase("UTF-8")){
                        utf8 = true;
                    }else if(e.startsWith("UTF-16")){
                        utf16 = true;
                    }else if(e.equalsIgnoreCase("EUC-KR")){
                        euckr = true;
                    }
                    if((utf8 && utf16) || (utf8 && euckr) || (euckr && utf16)){
                        break;
                    }
                }
                if(utf8 && !utf16 && !euckr){
                    group.selectToggle(controller.swt_utf8);
                }else if(!utf8 && utf16 && !euckr){
                    group.selectToggle(controller.swt_utf16);
                }else if(!utf8 && !utf16 && euckr){
                    group.selectToggle(controller.swt_euckr);
                }else{
                    group.selectToggle(controller.swt_show);
                }
            }
        },false,exclusive);
        //group.selectToggle(controller.swt_nothing);

        stage.setScene(scene);
        stage.show();
        //stage.initStyle(StageStyle.DECORATED);

        stage.show();
    }

    /**
     * http://code.makery.ch/blog/javafx-dialogs-official/
     * @param type Alert Type
     * @param title Alert Title: native view
     * @param header Alert Header title: big size
     * @param subtitle Alert subtitle: sub desc
     * @param label_text Alert label: upside of log
     * @param content Alert log
     */
    private void showFullDialog(Alert.AlertType type,String title, String header, String subtitle, String label_text, String content){
        Alert alert = new Alert(type);
        alert.setTitle(title);
        alert.setHeaderText(header);
        alert.setContentText(subtitle);

        Label label = new Label(label_text);

        TextArea textArea = new TextArea(content);
        textArea.setEditable(false);
        textArea.setWrapText(true);

        textArea.setMaxWidth(Double.MAX_VALUE);
        textArea.setMaxHeight(Double.MAX_VALUE);
        GridPane.setVgrow(textArea, Priority.ALWAYS);
        GridPane.setHgrow(textArea, Priority.ALWAYS);

        GridPane expContent = new GridPane();
        expContent.setMaxWidth(Double.MAX_VALUE);
        expContent.add(label, 0, 0);
        expContent.add(textArea, 0, 1);

        alert.getDialogPane().setExpandableContent(expContent);
        alert.getDialogPane().setExpanded(true);
        alert.getDialogPane().getStylesheets().add(style);

        alert.show();
    }
}
