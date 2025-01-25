SELECT pl.name FROM people AS pl
WHERE pl.id IN
(
    SELECT st.person_id FROM stars AS st
    WHERE st.movie_id IN
    (
        SELECT sts.movie_id FROM stars AS sts
        WHERE sts.person_id IN
        (
            SELECT pls.id FROM people AS pls
            WHERE pls.name = 'Kevin Bacon' AND pls.birth = 1958
        )
    )
)
AND pl.name != 'Kevin Bacon'
GROUP BY pl.name;

