/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7a;

import JDBCWrapper.*;
import java.util.ArrayList;
import java.sql.*;

/**
 *
 * @author cvalle
 */
public class Lab7a {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Initialize database connection
        String uri = "jdbc:mysql://localhost:3306/MedicalData";
        String username = "root";
        String password = "cpen436";
        Database db = new Database(uri, username, password);
        
        // Get the table
        Table table = db.getTable("demo08Q1");
        
        // Create an ArrayList of columns we want from the table
        ArrayList<String> cols = new ArrayList<>(2);
        cols.add("AGE");
        cols.add("AGE_COD");
        
        // Get the rows from the table
        RowSet rows = null;
        try
        {
            rows = table.getRows(null, cols);
        }
        catch (SQLException e)
        {
            System.out.println("Error getting rows: " + e);
            return;
        }
        
        // Do stuff with rows
        double total_age = 0;
        int num_ages = 0;
        for (Row r : rows.getRows())
        {
            String age = r.get(0).trim();
            String age_code = r.get(1).trim();
            
            if (age.isEmpty() || age_code.isEmpty()) continue;
            
            ++num_ages;
            
            switch (age_code) {
                case "DEC":
                    total_age += Double.parseDouble(age) * 10;
                    break;

                case "YR":
                    total_age += Double.parseDouble(age);
                    break;

                case "MON":
                    total_age += Double.parseDouble(age) / 12;
                    break;

                case "WK":
                    total_age += Double.parseDouble(age) / 52;
                    break;

                case "DY":
                    total_age += Double.parseDouble(age) / 365.25;
                    break;

                case "HR":
                    total_age += Double.parseDouble(age) / 8766;
                    break;

                default:
                    System.out.println("Unknown age code: " + age_code + ".");
                    break;
            }
        }
        
        System.out.println("The average age of patients in the database is " + (total_age / num_ages) + " years.");
    }
    
}
