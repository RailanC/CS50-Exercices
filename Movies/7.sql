SELECT rt.rating, mv.title FROM ratings AS rt JOIN movies AS mv ON rt.movie_id = mv.id WHERE year = 2010 ORDER BY rt.rating DESC, title; 
