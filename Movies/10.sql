SELECT pl.name FROM people AS pl
JOIN directors AS dt ON dt.person_id = pl.id
JOIN movies AS mv ON dt.movie_id = mv.id
JOIN ratings AS rt ON mv.id = rt.movie_id
WHERE rt.rating >= 9;
