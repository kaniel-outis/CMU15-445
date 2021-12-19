Select decade,count(*) as num
From (
	select (Cast ((date_year/10) as int)*10) || 's' as decade
	from release_info 
		inner join release on  release.id = release_info.release
	where release.status = 1 and date_year>=1900
)
Group by decade
Order by num desc;