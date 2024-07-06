IMPORTING DATA -
1. Read HELP from SQL Sever Management Studio -> Required Database -> Tasks -> Import Data 
2. Have to install SQL Setup Wizard in Developer Mode and select  , then importing data is simple
3. Import data easily from Import and Export Wizard in START menu

Code is contained in my SQL-QUERY

Following is snippet
/*
Covid 19 Data Exploration 

Skills used: Joins, CTE's, Temp Tables, Windows Functions, Aggregate Functions, Creating Views, Converting Data Types

*/


-- continents have their continents NULL
Select *
From PortfolioProject1..covid$
Where continent is not null 
order by 3,4



-- infection ratio and death ratio analysis country-wise , Shows what percentage of population infected with Covid
Select Location, date, total_cases, new_cases, total_deaths, population, cast( total_cases as float) / population as Infection_Ratio, cast(total_deaths as float) / total_cases as Death_Ratio
From PortfolioProject1..covid$
Where continent is not null 
and total_deaths is not null
and population is not null 
 and total_cases is not null 
order by 1,2


-- Total Cases vs Total Deaths
-- Shows likelihood of dying if you contract covid in your country
Select Location, date, total_cases,total_deaths, population , (total_deaths/total_cases)*100 as DeathPercentage
From PortfolioProject1..covid$
Where location like '%india%'
and continent is not null 
and total_deaths is not null
and total_cases is not null 
order by 1,2



-- Countries with Highest Infection Rate compared to Population
Select Location, Population, MAX(total_cases) as HighestInfectionCount,  Max((total_cases/population))*100 as PercentPopulationInfected
From PortfolioProject1..covid$
--Where location like '%states%'
Group by Location, Population
order by PercentPopulationInfected desc




-- Countries with Highest Death Coun
Select Location, MAX(cast(Total_deaths as int)) as TotalDeathCount
From PortfolioProject1..deaths
--Where location like '%states%'
Where continent is not null 
Group by Location
order by TotalDeathCount desc



-- BREAKING THINGS DOWN BY CONTINENT using CTE
-- Showing CONTINENTS with the highest death count per population
WITH MaxValuesPerLocation AS (
    SELECT continent,
           location,
           MAX(cast(total_deaths as int)) AS MaxLocationValue
    FROM PortfolioProject1..CovidDeaths
	where continent is not null
    GROUP BY continent, location
	
)
SELECT continent,
       SUM(MaxLocationValue) AS SumOfMaxPerContinent
FROM MaxValuesPerLocation
GROUP BY continent
order by SumOfMaxPerContinent desc




-- GLOBAL NUMBERS
Select SUM(new_cases) as total_cases, SUM(cast(new_deaths as int)) as total_deaths, SUM(cast(new_deaths as int))/SUM(New_Cases)*100 as DeathPercentage
From PortfolioProject1..CovidDeaths
where continent is not null 
and location like '%india%'
Group By location --date
order by 1,2




-- Total Population vs Vaccinations
-- Shows Percentage of Population that has recieved at least one Covid Vaccine
Select dea.continent, dea.location, dea.date, dea.population, vac.new_vaccinations
, SUM(CONVERT(bigint,vac.new_vaccinations)) OVER (Partition by dea.Location /*Order by dea.location, dea.Date*/) as RollingPeopleVaccinated, SUM(CONVERT(bigint,vac.new_vaccinations)) OVER (Partition by dea.Location  /*Order by dea.location, dea.Date*/)/population *100 as RollingPercentVaccinated
From PortfolioProject1..CovidDeaths dea
Join PortfolioProject1..CovidVaccinations vac
	On dea.location = vac.location
	and dea.date = vac.date
where dea.continent is not null 
order by  6 desc, 7 desc 






-- Using CTE to perform Calculation on Partition By in previous query
-- does HDI related to RollingVaccinationPercentage
With PopvsVac (Continent, Location, Date, Population, New_Vaccinations, RollingPeopleVaccinated,Human_development_index)
as
(
Select dea.continent, dea.location, dea.date, dea.population, vac.new_vaccinations
, SUM(CONVERT(bigint,vac.new_vaccinations)) OVER (Partition by dea.Location /*Order by dea.location, dea.Date*/) as RollingPeopleVaccinated,Human_development_index
--, (RollingPeopleVaccinated/population)*100
From PortfolioProject1..CovidDeaths dea
Join PortfolioProject1..CovidVaccinations vac
	On dea.location = vac.location
	and dea.date = vac.date
where dea.continent is not null 
--order by 2,3
)
Select *, (RollingPeopleVaccinated/Population)*100 as RollingPercentVaccinated
From PopvsVac
order by 7 desc , 8 desc





-- Using Temp Table to perform Calculation on Partition By in previous query
DROP Table if exists #PercentPopulationVaccinated
Create Table #PercentPopulationVaccinated
(
Continent nvarchar(255),
Location nvarchar(255),
Date datetime,
--Population numeric,
New_vaccinations numeric,
RollingPeopleVaccinated numeric
)

Insert into #PercentPopulationVaccinated
Select dea.continent, dea.location, dea.date, dea.new_vaccinations
, SUM(CONVERT(bigint,dea.new_vaccinations)) OVER (Partition by dea.Location /*Order by dea.location, dea.Date*/) as RollingPeopleVaccinated
--, (RollingPeopleVaccinated/population)*100
From PortfolioProject1..CovidVaccinations dea
--Join PortfolioProject1..CovidVaccinations vac
	--On dea.location = vac.location
	--and dea.date = vac.date
--where dea.continent is not null 
--order by 2,3

Select *--, (RollingPeopleVaccinated/Population)*100
From #PercentPopulationVaccinated





-- Creating View to store data for later visualizations
Create View PercentPopulationVaccinated as
Select dea.continent, dea.location, dea.date, dea.population, vac.new_vaccinations
, SUM(CONVERT(bigint,vac.new_vaccinations)) OVER (Partition by dea.Location /*Order by dea.location, dea.Date*/) as RollingPeopleVaccinated
--, (RollingPeopleVaccinated/population)*100
From PortfolioProject1..CovidDeaths dea
Join PortfolioProject1..CovidVaccinations vac
	On dea.location = vac.location
	and dea.date = vac.date
where dea.continent is not null 

Select *
From PercentPopulationVaccinated



WINDOWS FUNCTIONS :
ROW_NUMBER()
RANK()
DENSE_RANK()



STORED_PROCEDURES: 




TRIGGERS AND EVENTS :
