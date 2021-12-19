Select b.name,count(*) as num
From artist a,area as b
Where a.begin_date_year<1850 and a.area=b.id
Group by a.area
Order by num desc 
limit 10;