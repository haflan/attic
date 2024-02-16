import java.util.Scanner;

public class Matrix {
    private int columns;
    private int rows;
    private double[][] matrix;

    public Matrix(int rows, int columns){
        this.rows = rows;
        this.columns = columns;
        this.matrix = new double[rows][columns];
        this.initToZero();
    }

    public Matrix(int size){
        this(size, size);
    }

    private void initToZero(){
        for (int r = 0; r < this.rows; r++){
            for (int c = 0; c < this.columns; c++){
                this.matrix[r][c] = 0.0;
            }
        }
    }

    public void set(int row, int col, double value){
        this.matrix[row][col] = value;
    }

    public Matrix multiply(Matrix B){
        Matrix A = this;    // for readability
        if (A.columns != B.rows)
            return null;

        Matrix C = new Matrix(A.rows, B.columns);
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < columns; c++){
                C.matrix[r][c] = 0.0;
                for (int i = 0; i < this.columns; i++){
                    C.matrix[r][c] += A.matrix[r][i] * B.matrix[i][c];
                }
            }

        }
        return C;
    }

    @Override
    public String toString(){
        String toReturn = "";
        for (int r = 0; r < this.rows; r++) {
            for (int c = 0; c < this.columns; c++) {
                toReturn += String.format("%1.4f \t", this.matrix[r][c]);
            }
            toReturn += "\n";
        }
        return toReturn;
    }

    public Matrix getTransposed(){
        Matrix transposed = new Matrix(columns, rows);
        for (int r = 0; r < rows; r++){
            for (int c = 0; c < columns; c++){
                transposed.matrix[c][r] = this.matrix[r][c];
            }
        }
        return transposed;
    }

    public void insertMatrix(){
        Scanner reader = new Scanner(System.in);
        System.out.println("Insert the matrix elements col by col, row by row:");
        for (int r = 0; r < this.rows; r++){
            for (int c = 0; c < this.columns; c++){
                this.matrix[r][c] = Double.parseDouble(reader.next());
            }
        }
    }

    public int getColumns(){
        return this.columns;
    }

    public int getRows(){
        return this.rows;
    }

}
