#先找做一个tuple<id1,id2,num>  然后考虑限定条件 1）english 2）solo 3)year
With duos_list(id1,id2,num) as(
	select a1.artist as id1,
	       a2.artist as id2,
               count(*)  as num
	from artist_credit_name a1
	inner join artist_credit_name a2 on a1.artist_credit = a2.artist_credit
	inner join artist_credit a3 on a3.id = a2.artist_credit
	inner join release r1 on r1.artist_credit = a3.id
	inner join language l1 on l1.id = r1.language
	inner join artist a4 on a4.id = a1.artist 
	inner join artist a5 on a5.id = a2.artist 
	inner join artist_type a6 on a6.id = a4.type
	inner join artist_type a7 on a7.id = a5.type

	where a4.name < a5.name and l1.name = 'English'
	and   a6.name = "Person"
	and   a7.name = "Person"
	and   a4. begin_date_year >1960
	and   a5. begin_date_year >1960
    	
	group by a1.artist,a2.artist
)

Select * 
From (
	Select row_number () over(
	order by num desc,
                    a1.name,
                    a2.name
	) as rank, a1.name as name1, a2.name as name2, num
	From duos_list as d
	    inner join artist a1 on d.id1 = a1.id
            inner join artist a2 on d.id2 = a2.id
)
where name1 = 'Dr. Dre'
    and name2 = 'Eminem';

