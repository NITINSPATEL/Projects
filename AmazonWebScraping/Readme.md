# Files #

## Overview ##
- PANDAS
    - read_csv(r"csv_path") , have imp. parameters : header , index , names, sep,   
    - read_table , read_json , read_excel (para : sheet_name)
    - pd.set_option('display.max.rows',num_max_rows+2) ,  pd.set_option('display.max.columns',num_max_cols+2)  : changes the deafualt view of dataframe , having dots between all 
    - df.info() , df.shape , df.head(10) , df.tail(10) , df['col_name']
    - df.loc[row_index_number] , df.iloc[row_index_number] , iloc always focus on index number (even after changing indexing column) , loc can access different columns also
    - slicing , indexing , condition filtering can be done in df as in arrays df[df['col'] <= 10 ] ,etc.
    -  df operations :  df[df['col'].isin(list) ] , df[df['col'].str.contains('text') ] , df.set_index('col_name') , df.filter(items=[list] , axis = 0/1) {1 by default , which is for columns} ,  df.filter(like=[list] , axis = 0/1) , df.sort_values(by=[list of columns], ascending = [false,true, etc.]) {first element of list has highest priority}
