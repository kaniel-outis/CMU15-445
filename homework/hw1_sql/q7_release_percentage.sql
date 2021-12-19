With newtab(year_month,release_num)
as(
	select cast(date_year as archer) || '.' || 
	(case when date_month < 10 then '0'
	else ''
	end) ||
	cast(date_month as archer) as year_month,
	       count(*) as release_num
 	from release_info 
	where ((date_year=2019 and date_month >=7) or (date_year=2020 and date_month <=7))
	Group by year_month
)

Select year_month,
	round((
		release_num*100.0 / (
			select sum(release_num) from newtab
		)
	),2)
From newtab;
