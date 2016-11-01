/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7c;

import JDBCWrapper.*;
import java.sql.SQLException;

/**
 *
 * @author cvalle
 */
public class Lab7c {

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
        
        RowSet rows = null;
        
        // Test 1
        try {
            rows = table.executeSQL(
                            "SELECT ISR, COUNT(ISR) AS TOTAL " +
                            "FROM DRUG08Q1 " +
                            "WHERE DRUGNAME LIKE 'PROPRANOLOL HCL' " +
                            "GROUP BY ISR;");
        }
        catch (SQLException e) {
            System.out.println("Error getting rows: " + e);
            return;
        }
        
        // Print out the ISRs
        System.out.println("ISRS from Test 1:");
        for (Row r : rows.getRows())
            System.out.println(r.get(0) + " " + r.get(1));
        
        // Test 2
        try {
            rows = table.executeSQL(
                            "SELECT COUNT(TMP.ISR) AS count " +
                            "FROM (" +
                                "SELECT DRUG08Q1.ISR " +
                                "FROM DRUG08Q1,REAC08Q1 " + 
                                "WHERE DRUG08Q1.ISR = REAC08Q1.ISR " +
                                "AND DRUGNAME = 'SELBEX' " +
                                "AND PT = 'BRONCHITIS' " +
                                "GROUP BY DRUG08Q1.ISR" +
                            ") AS TMP;");
        }
        catch (SQLException e) {
            System.out.println("Error getting rows: " + e);
            return;
        }
        
        // Print out the ISRs
        System.out.println("Count from Test 2:");
        for (Row r : rows.getRows())
            System.out.println(r.get(0));
    }
    
}
