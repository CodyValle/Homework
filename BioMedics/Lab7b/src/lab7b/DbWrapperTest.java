/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7b;

import JDBCWrapper.Row;
import java.util.List;

/**
 *
 * @author cvalle
 */
public class DbWrapperTest {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // Initialize the wrapper
        DbWrapper wrapperTest = new DbWrapper();
        
        // Get rows form the drug table
        List<Row> drugs = wrapperTest.getDrugs(5477131);
        
        // We are expecting 11 rows for this ISR
        if (drugs.size() != 11)
            System.out.println("Error! Expected 11 rows, found " + drugs.size() + ".");
        else
            System.out.println("Drug table success.");
        
        // Get rows form the reaction table
        List<Row> reacs = wrapperTest.getDrugs(5477131);
        
        // We are expecting 2 rows for this ISR
        if (reacs.size() != 11)
            System.out.println("Error! Expected 11 rows, found " + reacs.size() + ".");
        else
            System.out.println("Reaction table success.");
        
        // Test complete        
    }
}
