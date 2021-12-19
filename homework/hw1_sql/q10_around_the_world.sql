With newtab1 as(
	Select row_number() over(
		order by a.id asc	
	) as idx, a.name
	From artist_alias a
		inner join artist b on b.id = a.artist
	Where b.name = 'The Beatles'
),
newtab2 as(
	select idx, name
	from newtab1
	where idx = 1
	union all
	select newtab1.idx, newtab2.name || ', ' || newtab1.name
	from newtab1
		inner join newtab2 on newtab1.idx = newtab2.idx + 1
)

Select name 
From newtab2
order by idx desc
limit 1;
