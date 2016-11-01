package JDBCWrapper;

import java.util.*;

public class RowSet
{
  private ArrayList<Row> arraylist = new ArrayList<>();

  public RowSet() {
  }

  public void add( Row row ) {
    arraylist.add( row );
  }

  public int length() {
    return arraylist.size();
  }

  public Row get( int which ) {
    return arraylist.get( which );
  }

  public ArrayList<Row> getRows() {
    return arraylist;
  }
  
  public void mergeRowSet(RowSet rows)
  {
  	for(int i=0; i<rows.length(); i++) {
  		
  		arraylist.add(rows.get(i));
  	}
  }
}
