package application;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import javafx.fxml.FXML;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class MainController {
    @FXML
    private TextField cheminFichierEntree;

    @FXML
    private TextField cheminFichierSortie;

    @FXML
    private Button boutonSlowConvexHull;

    @FXML
    private Button boutonConvexHull1;

    @FXML
    private Button boutonConvexHull2;

    @FXML
    private Button boutonConvexHull3;

    @FXML
    private Button boutonRapidConvexHull;

    public void initialize() {}

    @FXML
    public void clickSlowConvexHull() {
        System.out.println("SlowConvexHull");
        new MyClass().findConvexHull(cheminFichierEntree.getText(), cheminFichierSortie.getText(), 1, 0);
    }

    @FXML
    public void clickConvexHull1() {
        System.out.println("ConvexHull (tri par tas avec un arbre)");
        new MyClass().findConvexHull(cheminFichierEntree.getText(), cheminFichierSortie.getText(), 2, 1);
    }

    @FXML
    public void clickConvexHull2() {
        System.out.println("ConvexHull (tri par tas avec un tableau)");
        new MyClass().findConvexHull(cheminFichierEntree.getText(), cheminFichierSortie.getText(), 2, 2);
    }

    @FXML
    public void clickConvexHull3() {
        System.out.println("ConvexHull (tri par sélection)");
        new MyClass().findConvexHull(cheminFichierEntree.getText(), cheminFichierSortie.getText(), 2, 3);
    }

    @FXML
    public void clickRapidConvexHull() {
        System.out.println("RapidConvexHull");
        new MyClass().findConvexHull(cheminFichierEntree.getText(), cheminFichierSortie.getText(), 3, 0);
    }

    @FXML
    public void clickAfficherSolution() {
        System.out.println(cheminFichierSortie.getText());
        String msg ="";

        try {
            File myObj = new File(cheminFichierSortie.getText());
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                msg = msg + data + "\r\n";
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("Le chemin du fichier n'est pas valide!");
            e.printStackTrace();
        }

        Group root = new Group();
        Scene scene = new Scene(root, 500, 300);

        Label label = new Label(msg);
        root.getChildren().add(label);


        Stage stage = new Stage();
        stage.setScene(scene);
        stage.setTitle("My modal window");
        stage.show();

    }

}