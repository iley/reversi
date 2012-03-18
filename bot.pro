move(Field, Color, Row, Col) :-
  correct_moves(Field, Color, Moves),
  Moves = [First|Rest],
  [Point|NewField] = First,
  point_row(Point, Row),
  point_col(Point, Col).
