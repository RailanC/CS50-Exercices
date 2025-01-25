SELECT pl.name FROM people AS pl
JOIN stars AS st ON st.person_id = pl.id
WHERE EXISTS (SELECT 1 FROM movies AS mv WHERE mv.id = st.movie_id AND mv.year = 2004)
ORDER BY pl.birth;
