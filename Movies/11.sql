SELECT mv.title FROM movies AS mv
JOIN stars AS st ON st.movie_id = mv.id
JOIN people As pl ON st.person_id = pl.id
JOIN ratings AS rt ON mv.id = rt.movie_id
WHERE pl.name = 'Chadwick Boseman' ORDER BY rt.rating DESC LIMIT 5;
