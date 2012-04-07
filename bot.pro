move(Field, Color, Row, Col) :-
  set_color(Color),
  int_min(Alpha),
  int_max(Beta),
  alphabeta(pos(Field, Color, []), Alpha, Beta, BestPos, _),
  BestPos = pos(_, _, [Point]),
  point_row(Point, Row),
  point_col(Point, Col).

% FIXME use retract here
set_color(Color) :-
  my_color(_), !
;
  assert(my_color(Color)).

max_level(3).

min_move(pos(_, Color, _)) :-
  not(my_color(Color)).

max_move(pos(_, Color, _)) :-
  my_color(Color).

level(Pos, Level) :-
  Pos = pos(_,_,History),
  length(History, Level).

score(pos(Field, Color, _), Score) :-
  score(Field, Color, Score).

alphabeta(Pos, Alpha, Beta, GoodPos, Score) :-
  %trace,
  level(Pos, Level),
  max_level(MaxLevel),
  Level =< MaxLevel,
  correct_moves(Pos, Moves), !,
  better(Moves, Alpha, Beta, GoodPos, Score)
;
  score(Pos, Score).

better([Pos|Rest], Alpha, Beta, GoodPos, GoodScore) :-
  alphabeta(Pos, Alpha, Beta, _, Score),
  good_enough(Rest, Alpha, Beta, Pos, Score, GoodPos, GoodScore).

good_enough([], _, _, Pos, Score, Pos, Score) :- !.

good_enough(_, Alpha, Beta, Pos, Score, Pos, Score) :-
  min_move(Pos), Score > Beta, !
;
  max_move(Pos), Score < Alpha, !.

good_enough(List, Alpha, Beta, Pos, Score, GoodPos, GoodScore) :-
  new_limits(Alpha, Beta, Pos, Score, NewAlpha, NewBeta),
  better(List, NewAlpha, NewBeta, Pos1, Score1),
  select(Pos, Score, Pos1, Score1, GoodPos, GoodScore).

new_limits(Alpha, Beta, Pos, Score, Score, Beta) :-
  min_move(Pos), Score > Alpha, !.

new_limits(Alpha, Beta, Pos, Score, Alpha, Score) :-
  max_move(Pos), Score < Beta, !.

new_limits(Alpha, Beta, _, _, Alpha, Beta).

select(Pos, Score, Pos1, Score1, Pos, Score) :-
  min_move(Pos), Score > Score1, !
;
  max_move(Pos), Score < Score1, !.

select(_, _, Pos1, Score1, Pos1, Score1).
