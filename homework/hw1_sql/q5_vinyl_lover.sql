Select distinct  r1.name
from artist_credit_name a1
	inner join artist_credit a2 on a2.id = a1. artist_credit
	inner join release r1 on r1.artist_credit = a2.id
	inner join release_info r2 on r2.release =r1.id
	inner join medium m1 on m1.release = r1.id
	inner join medium_format m2 on m1.format = m2.id
Where a1.name = 'Coldplay' and m2.name like '%Vinyl'
Order by date_year,date_month,date_day;