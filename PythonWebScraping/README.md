# Files #

## Overview ##
important functions : <br>
- os.listdir(path of dir)
- os.makedirs(path + dir name)
- os.path.exists(path)
- shutil.move(source path, destination path)   // for moving file from source to dest
- WebScraping Tutorial
  - [scrapethissite.com](https://www.scrapethissite.com/pages/forms/) , right click on any HTML page for inspect , you will get correponding code for every HTML wepage element and vice-versa
  - beautifulsoup (from bs4) and requests : page = requests.get(url) ; soup = BeautifulSoup(page.text,'html') ; soup.prettify() ;
  - soup.find('text') , soup.find_all('text') , soup.find_all('text',class_ = 'class1') ;   
