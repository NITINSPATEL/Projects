# Files #

## Overview ##
- PANDAS
    - read_csv(r"csv_path") , have imp. parameters : header , index , names, sep,   index_col 
    - read_table , read_json , read_excel (para : sheet_name)
    - pd.set_option('display.max.rows',num_max_rows+2) ,  pd.set_option('display.max.columns',num_max_cols+2)  : changes the deafualt view of dataframe , having dots between all 
    - df.info() , df.shape , df.head(10) , df.tail(10) , df['col_name']
    - df.loc[row_index_number] , df.iloc[row_index_number] , iloc always focus on index number , inherently given to rows (even after changing indexing column) , loc can access different columns also  , loc can access multiple indexing columns as well, loc searches from first column onwards in dataframe while accessing (mismatch of country and continent), loc is level based accessing (first, second index and so on), can do slicing ,etc. in both  
    - slicing , indexing , condition filtering can be done in df as in arrays df[df['col'] <= 10 ] ,etc.
    -  df operations :  df[df['col'].isin(list) ] , df[df['col'].str.contains('text') ] , df.set_index('col_name')  {multiple columns can be there} , df.reset_index(inplace=true) , df.filter(items=[list] , axis = 0/1) {1 by default , which is for columns} ,  df.filter(like=[list] , axis = 0/1) , df.sort_values(by=[list of columns], ascending = [false,true, etc.]) {first column of list has highest priority}
    -  indexing : df.sort_index(ascending=[list IFF multiple column are list]) , iloc is number specific accessing (index umber is fixed , can't be changed) , loc access level by level all indexing columns
    -  aggregating  : 
