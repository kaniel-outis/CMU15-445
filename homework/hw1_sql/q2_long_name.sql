select work.name,work_type.name
from work_type,work 
	inner join(
	select work.type,
	       max(length(work.name)) as mx
	from work
	group by work.type
) as newtab on  newtab.type = work.type and mx = length(work.name)
Where newtab.type = work_type.id
order by work.type asc,
	work.name asc;
