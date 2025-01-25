SELECT pl.name FROM people AS pl JOIN stars AS st ON st.person_id = pl.id JOIN movies AS mv ON mv.id = st.movie_id WHERE mv.title LIKE 'Toy Story';
