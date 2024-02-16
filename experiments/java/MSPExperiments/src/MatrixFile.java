import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MatrixFile {
    File mFile;
    Scanner fileReader;
    Matrix matrixRead;
    Pattern pattern;
    Matcher matcher;

    public MatrixFile(String filename){
        this.mFile = new File(filename);
        pattern = Pattern.compile("(\\d)x(\\d)");
        try {
            int rows = 0;   int cols = 0;
            this.fileReader = new Scanner(this.mFile);
            String line = fileReader.nextLine();
            matcher = pattern.matcher(line);
            if (matcher.matches()){
                rows = Integer.parseInt(matcher.group(1));
                cols = Integer.parseInt(matcher.group(2));
            } else {
                throw new Exception("Matrix in wrong format");
            }
            this.matrixRead = new Matrix(rows, cols);
            String newValue;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < rows; c++) {
                    if ((newValue = fileReader.next()) != null){
                        this.matrixRead.set(r, c, Double.parseDouble(newValue));
                    } else {
                        throw new Exception("Matrix in wrong format");
                    }
                }
            }


        }
        catch (FileNotFoundException e){
            e.printStackTrace();
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    public Matrix getMatrix(){
        return this.matrixRead;
    }

}
