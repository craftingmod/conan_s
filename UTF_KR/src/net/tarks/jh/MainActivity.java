package net.tarks.jh;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.prefs.Preferences;

public class MainActivity extends Application {

    private Scene scene;
    private Parent root;
    private MainController controller;
    private Preferences pref;

    @Override
    public void start(Stage stage) {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("ui.fxml"));
        root = null;
        try {
            root = loader.load();
        } catch (IOException e) {
            e.printStackTrace();
        }
        stage.setTitle("UTF/EUC");
        stage.setResizable(true);
        stage.setAlwaysOnTop(false);
        stage.setIconified(false);
        scene = new Scene(root, 180, 160);

        controller = loader.getController();
        pref = Preferences.userNodeForPackage(this.getClass());
        /**
         * Insert code
         */
        controller.btn.setPrefWidth(200);

        final ToggleGroup group = new ToggleGroup();
        controller.swt_utf8.setToggleGroup(group);
        controller.swt_cp949.setToggleGroup(group);
        controller.swt_euckr.setToggleGroup(group);
        controller.btn.setOnMouseClicked(event -> {
            System.out.printf("Hello %.2f\n",Math.random()*100);
        });

        stage.setScene(scene);
        stage.show();
        //stage.initStyle(StageStyle.DECORATED);

        stage.show();
    }
}
