SELECT mv.title FROM movies AS mv
JOIN stars AS st ON st.movie_id = mv.id
JOIN people As pl ON st.person_id = pl.id
WHERE pl.name IN ('Jennifer Lawrence', 'Bradley Cooper')
GROUP BY mv.title
HAVING COUNT(*)>1;
