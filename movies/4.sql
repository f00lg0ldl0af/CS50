SELECT COUNT(title) from movies WHERE id IN (SELECT movie_id  from ratings WHERE rating = 10.0);