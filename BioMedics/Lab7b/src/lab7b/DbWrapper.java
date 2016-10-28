/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7b;

import JDBCWrapper.Database;
import JDBCWrapper.RowSet;
import JDBCWrapper.Table;
import JDBCWrapper.Row;

import java.sql.SQLException;
import java.util.ArrayList;

/**
 *
 * @author cvalle
 */
public class DbWrapper {
    
    private final Database db;
    
    DbWrapper() {
        // Initialize database connection
        String uri = "jdbc:mysql://localhost:3306/MedicalData";
        String username = "root";
        String password = "cpen436";
        db = new Database(uri, username, password);
    }
    
    public ArrayList<Row> getDrugs(Integer ISR) {
        // Get the table
        Table tb = db.getTable("drug08q1");
        
        // Create an ArrayList of columns we want from the table
        ArrayList<String> cols = new ArrayList<>(12);
        cols.add("ISR");
        cols.add("DRUG_SEQ");
        cols.add("ROLE_COD");
        cols.add("DRUGNAME");
        cols.add("VAL_VBM");
        cols.add("ROUTE");
        cols.add("DOSE_VBM");
        cols.add("DECHAL");
        cols.add("RECHAL");
        cols.add("LOT_NUM");
        cols.add("EXP_DT");
        cols.add("NDA_NUM");
        
        // Get the rows from the table
        RowSet rows = null;
        try {
            rows = tb.getRows("ISR=" + ISR, cols);
        }
        catch (SQLException e) {
            System.out.println("Error getting rows: " + e);
            return null;
        }
        
        return rows.getRows();
    }
    
    public ArrayList<Row> getReactions(Integer ISR) {
        // Get the table
        Table tb = db.getTable("reac08q1");
        
        // Create an ArrayList of columns we want from the table
        ArrayList<String> cols = new ArrayList<>(2);
        cols.add("ISR");
        cols.add("PT");
        
        // Get the rows from the table
        RowSet rows = null;
        try {
            rows = tb.getRows("ISR=" + ISR, cols);
        }
        catch (SQLException e) {
            System.out.println("Error getting rows: " + e);
            return null;
        }
        
        return rows.getRows();
    }
}






















