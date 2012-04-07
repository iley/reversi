move(Field, Color, Row, Col) :-
  %trace,
  set_color(Color),
  minmax(pos(Field, Color, []), 0, BestPos, _),
  BestPos = pos(_, _, [Point]),
  point_row(Point, Row),
  point_col(Point, Col).

% FIXME use retract here
set_color(Color) :-
  my_color(_), !
;
  assert(my_color(Color)).

max_level(3).

score(pos(Field, Color, _), Score) :-
  score(Field, Color, Score).

minmax(Pos, Level, BestPos, Score) :-
  max_level(MaxLevel),
  Level =< MaxLevel,
  correct_moves(Pos, Moves), !,
  NewLevel is Level + 1,
  best(Moves, NewLevel, BestPos, Score)
;
  score(Pos, Score).

best([Pos], Level, Pos, Score) :-
  minmax(Pos, Level, _, Score), !.

best([Pos1|Rest], Level, BestPos, BestScore) :-
  minmax(Pos1, Level, _, Score1),
  best(Rest, Level, Pos2, Score2),
  select(Pos1, Score1, Pos2, Score2, BestPos, BestScore).

select(Pos1, Score1, Pos2, Score2, Pos1, Score1) :-
  min_move(Pos1), Score1 < Score2, !
;
  max_move(Pos2), Score1 > Score2, !.

select(Pos1, Score1, Pos2, Score2, Pos2, Score2).

min_move(pos(_, Color, _)) :-
  not(my_color(Color)).

max_move(pos(_, Color, _)) :-
  my_color(Color).
