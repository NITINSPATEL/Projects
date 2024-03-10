/*

Cleaning Data in SQL Queries

*/


Select *
From PortfolioProject1.dbo.NashvilleHousing

--------------------------------------------------------------------------------------------------------------------------

-- Standardize Date Format


Select SaleDate,CONVERT(Date,SaleDate)
from  PortfolioProject1.dbo.NashvilleHousing

Select saleDateConverted, CONVERT(Date,SaleDate)
From PortfolioProject1.dbo.NashvilleHousing


Update PortfolioProject1.dbo.NashvilleHousing
SET SaleDate = CONVERT(Date,SaleDate)

-- If it doesn't Update properly , no logical explanation of why sabove method works sometimes , and sometimes not 

ALTER TABLE NashvilleHousing
Add SaleDateConverted Date;

Update PortfolioProject1.dbo.NashvilleHousing
SET SaleDateConverted =CONVERT(Date,SaleDate)


 --------------------------------------------------------------------------------------------------------------------------

-- Populate Property Address data


Select PropertyAddress
from PortfolioProject1.dbo.NashvilleHousING
Where PropertyAddress is null


Select *
From PortfolioProject1.dbo.NashvilleHousing
--Where PropertyAddress is null
order by ParcelID


--Same ParcelID -> Same PropertyAddress , found it by analysing data , use following code for verifying conjectured relations

-- is null -> if a.PropertyAdr. is null then use value of b.property addr. and make a new column

Select a.UniqueID,b.UniqueID,a.ParcelID, a.PropertyAddress, b.ParcelID, b.PropertyAddress, ISNULL(a.PropertyAddress,b.PropertyAddress)
From PortfolioProject1.dbo.NashvilleHousing a
JOIN PortfolioProject1.dbo.NashvilleHousing b
	on a.ParcelID = b.ParcelID
	AND a.[UniqueID ] <> b.[UniqueID ]
Where a.PropertyAddress is null


-- The WHERE clause filters individual rows before they are grouped, so it can't directly filter based on aggregated values like counts. That's why we use the HAVING clause, which is specifically designed to filter results after grouping and aggregation has been performed.
--can't directly reference an alias defined in the SELECT clause within the WHERE clause. SQL doesn't allow this because the WHERE clause is evaluated before the SELECT clause
SELECT ParcelId, COUNT(ParcelID) AS counting
FROM PortfolioProject1.dbo.NashvilleHousing
GROUP BY ParcelID
HAVING COUNT(ParcelID) = 1


--use alias when JOINING to update 
Update a
SET PropertyAddress = ISNULL(a.PropertyAddress,b.PropertyAddress)  -- ISNULL(a.PropertyAddress,'No Address')
From PortfolioProject1.dbo.NashvilleHousing a
JOIN PortfolioProject1.dbo.NashvilleHousing b
	on a.ParcelID = b.ParcelID
	AND a.[UniqueID ] <> b.[UniqueID ]
Where a.PropertyAddress is null




--------------------------------------------------------------------------------------------------------------------------

-- Breaking out Address into Individual Columns (Address, City, State)


Select PropertyAddress
From PortfolioProject1.dbo.NashvilleHousing
--Where PropertyAddress is null
--order by ParcelID

-- IN SQL , 1-based indexing , not 0-based indexing
-- can put strings in char index also , but be aware of fact that charindex returns -1 if no pattern found in current string , u can;t acces negative index in SQL , as in Python 
SELECT
SUBSTRING(PropertyAddress, 1, CHARINDEX(',', PropertyAddress) -1 ) as Address
, SUBSTRING(PropertyAddress, CHARINDEX(',', PropertyAddress) + 1, LEN(PropertyAddress)) as Address


--example to use if-else and charindex
SELECT
    CASE WHEN CHARINDEX('FOX', PropertyAddress) > 0
         THEN SUBSTRING(PropertyAddress, 1, CHARINDEX('FOX', PropertyAddress) - 1)
         ELSE PropertyAddress
    END as Address1,
    CASE WHEN CHARINDEX(',', PropertyAddress) > 0
         THEN SUBSTRING(PropertyAddress, CHARINDEX(',', PropertyAddress) + 1, LEN(PropertyAddress))
         ELSE ''
    END as Address2
FROM
    PortfolioProject1.dbo.NashvilleHousing



--Required:
--You must use semicolons to separate multiple SQL statements executed one after the other (e.g., SELECT followed by UPDATE).
--Optional (But Recommended):
--For a single SQL statement, the semicolon might not be strictly necessary in some environments. However, using them consistently is highly recommended


ALTER TABLE PortfolioProject1.dbo.NashvilleHousing
Add PropertySplitAddress Nvarchar(255);


Update PortfolioProject1.dbo.NashvilleHousing
SET PropertySplitAddress = SUBSTRING(PropertyAddress, 1, CHARINDEX(',', PropertyAddress) -1 )


ALTER TABLE PortfolioProject1.dbo.NashvilleHousing
Add PropertySplitCity2 Nvarchar(255);

Update PortfolioProject1.dbo.NashvilleHousing
SET PropertySplitCity2=SUBSTRING(PropertyAddress, CHARINDEX(',', PropertyAddress) + 1 , LEN(PropertyAddress))




Select *
From PortfolioProject1.dbo.NashvilleHousing





Select OwnerAddress
From PortfolioProject1.dbo.NashvilleHousing


UPDATE PortfolioProject1.dbo.NashvilleHousing
SET owneraddress = '1808  FOX CHASE DR, GOODLETTSVILLE, TN'
WHERE uniqueid = 2045 ;



SELECT PARSENAME('ServerName.DatabaseName.SchemaName.ObjectName',1) -- returns `DatabaseName
--The PARSENAME function in SQL is used to return the specified part of an object name. The object name can have a maximum of four parts: server.database.schema.object

select 
PARSENAME(owneraddress,1)
from portfolioproject1.dbo.NashvilleHousing


--PARSENAME is specifically designed to parse object names in a database context, which follow a standard format with periods (".") as separators
-- alternative to substring formula implemenatation
--The PARSENAME function splits the object name into parts based on the “.” delimiter. It considers the rightmost part as part 1, the second rightmost part as part 2, and so on.
Select
PARSENAME(REPLACE(OwnerAddress, ',', '.') , 3)
,PARSENAME(REPLACE(OwnerAddress, ',', '.') , 2)
,PARSENAME(REPLACE(OwnerAddress, ',', '.') , 1)
From PortfolioProject1.dbo.NashvilleHousing



ALTER TABLE portfolioproject1.dbo.NashvilleHousing
Add OwnerSplitAddress Nvarchar(255);


ALTER TABLE  portfolioproject1.dbo.NashvilleHousing
Add OwnerSplitCity Nvarchar(255);

ALTER TABLE  portfolioproject1.dbo.NashvilleHousing
Add OwnerSplitState Nvarchar(255);


Update  portfolioproject1.dbo.NashvilleHousing
SET OwnerSplitAddress = PARSENAME(REPLACE(OwnerAddress, ',', '.') , 3)

Update  portfolioproject1.dbo.NashvilleHousing
SET OwnerSplitCity = PARSENAME(REPLACE(OwnerAddress, ',', '.') , 2)



Update  portfolioproject1.dbo.NashvilleHousing
SET OwnerSplitState = PARSENAME(REPLACE(OwnerAddress, ',', '.') , 1)



Select *
From PortfolioProject1.dbo.NashvilleHousing




--------------------------------------------------------------------------------------------------------------------------


-- Change Y and N to Yes and No in "Sold as Vacant" field


Select Distinct(SoldAsVacant), Count(SoldAsVacant)
From PortfolioProject1.dbo.NashvilleHousing
Group by SoldAsVacant
order by 2


-- since Yes, No are most populated , Y/N are typo. Need to change them to Yes/No 

Select SoldAsVacant
, CASE When SoldAsVacant = 'Y' THEN 'Yes'
	   When SoldAsVacant = 'N' THEN 'No'
	   ELSE SoldAsVacant
	   END
From PortfolioProject1.dbo.NashvilleHousing


Update portfolioproject1.dbo.NashvilleHousing
SET SoldAsVacant = CASE When SoldAsVacant = 'Y' THEN 'Yes'
	   When SoldAsVacant = 'N' THEN 'No'
	   ELSE SoldAsVacant
	   END






-----------------------------------------------------------------------------------------------------------------------------------------------------------

-- Remove Duplicates  , not recommended , DATA is GOLD , we shouldn't delete data 

WITH RowNumCTE AS(
Select *,
	ROW_NUMBER() OVER (
	PARTITION BY ParcelID,             -- these parameters (over which partition is done) , if same , they belong to same datatset , adjacent ones are duplicate , row_num =2 just after row_num=1 , then both are duplicated , similarly for higher numbers
				 PropertyAddress,
				 SalePrice,
				 SaleDate,
				 LegalReference
				 ORDER BY
					UniqueID
					) row_num

From PortfolioProject1.dbo.NashvilleHousing
--order by ParcelID


)
--DELETE
Select *
From RowNumCTE
Where row_num > 1
Order by PropertyAddress



Select *
From PortfolioProject1.dbo.NashvilleHousing




---------------------------------------------------------------------------------------------------------

-- Delete Unused Columns , MOSTLY USED WHEN WE use VIEWS 



Select *
From PortfolioProject1.dbo.NashvilleHousing


ALTER TABLE PortfolioProject1.dbo.NashvilleHousing
DROP COLUMN PropertySplitCity2















-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------















