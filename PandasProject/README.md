# Files #
DataCleaning.ipynb
CustomerCallListCleaning.xlsx (Dataset for DataCleaning using Pandas)

## NOTE ##
- The objective of DataCleaning project is to clean the data , remove duplicates , remove null values row ,  not useful column , standardised column values , remove unnecesary special characters from column values . Cleaned data then can be suitably used for further analysis and AI-ML modelling part 
- The objective of DataExploration project is to  
- Jupyter Notebook codes are self-explanatory and easily understandable

## Overview of Pandas  ##
- read_csv(r"csv_path") , have imp. parameters : header , index , names, sep,   index_col 
- read_table , read_json , read_excel (para : sheet_name)
- pd.set_option('display.max.rows',num_max_rows+2) ,  pd.set_option('display.max.columns',num_max_cols+2)  : changes the deafualt view of dataframe , having dots between all 
- df.info() , df.shape , df.head(10) , df.tail(10) , df['col_name']
- df.loc[row_index_number] , df.iloc[row_index_number] , iloc always focus on index number , inherently given to rows (even after changing indexing column) , loc can access different columns also  , loc can access multiple indexing columns as well, loc searches from first column onwards in dataframe while accessing (mismatch of country and continent), loc is level based accessing (first, second index and so on), can do slicing ,etc. in both   , usage - df.loc[row_index, "ColName"] == ColValue  
- slicing , indexing , condition filtering can be done in df as in arrays df[df['col'] <= 10 ] ,etc.
-  df operations :  df[df['col'].isin(list) ] , df[df['col'].str.contains('text') ] , df.set_index('col_name')  {multiple columns can be there} , df.reset_index(inplace=true) , df.filter(items=[list] , axis = 0/1) {1 by default , which is for columns} ,  df.filter(like=[list] , axis = 0/1) , df.sort_values(by=[list of columns], ascending = [false,true, etc.]) {first column of list has highest priority}
-  indexing : df.sort_index(ascending=[list IFF multiple column are list]) , iloc is number specific accessing (index umber is fixed , can't be changed) , loc access level by level all indexing columns
-  aggregating  : gr_df = df.groupby(['col1','col2']) , gr_df.mean() - takes mean of all numerical columns in a group of grouped dataframe , gf_df.count() - cpunts no. of rows in every grouped thing , gf_df.min() and gf.df.max()  for min and max values in each column , gf_df.sum() , gf_df.agg({'col1':['mean' , 'max', etc. ] , 'col2':[] , etc.}) , gf_df.describe() 
-   merge  : df1.merge(df2) - df1 is left and df2 is right, by default does inner join , sees for same col names for merging by default ; df1.merge(df2, how = 'inner' , on = ['col1',etc.] ) ; how = outer / left / right / cross (cartesian product) ;    "on" columns are not repeated in merged dataframe
-  join : df1.join(df2) ; "on"  , "lsuffix", "rsuffix" are necessary to mention  {lsuffix = '_Left', etc.} , all columns are repeated in joined df ; "on" is not needed if we use df.set_index for both left and right df ; using set_index results are very similar as that of merge
-  concatenate :  pd.concat([df1,df2]) , df2's rows are concatenated to df1's rows ; pd.concat(axis=0 by default , axis=1 -> side by side concatenation) ; join parameter for deciding which columns to take in concatenated df
-  df1.append(df2) == pd.concat([df1,df2])
-  visualizations : matplotlib.style.available ; plt.style.use('any element of the previous output') 
-  visual1 : df.plot() / df['col'].plot()  / df.plot.barh (for horizontal bar chart) ; parameters ->  kind = 'line' / 'bar'   ,stacked = true ,  subplots = True/false , title , xlabel , ylabel ,
-  visual2 : df.plot.scatter() ; parameters -> x = 'col1' , y='col2' , s = 50 , c= 'yellow' ,
-  visual3 : df.plot.hist() ; parameters -> bins = 20
-  visual4 : df.boxplot() ;  0,25,50,75,100 percentile are 5 horizontal lines
-  visual5 : df.plot.area() ; parmeters - figsize = (10,20)
-  visual6 : df.plot.pie() ; parameters - y = 'col_name' , figsize = (10,20) 
