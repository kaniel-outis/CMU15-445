Select a.name,count(distinct b.name) as num
From artist as a, artist_alias as b, area as c
Where a.id = b.artist and a.area = c.id and a.begin_date_year > 1950 and c.name ='United Kingdom'
Group by a.id
Order by num desc
Limit 10;


