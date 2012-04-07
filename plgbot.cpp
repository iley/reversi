#include "plgbot.hpp"
#include "reversi.hpp"
#include "bot.hpp"
#include <prolog/prolog.hpp>
#include <prolog/library/library.hpp>
#include <cassert>

// correct_moves(+pos(Field,Color,History),-Moves)
static bool predicateCorrectMoves(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    using namespace PlgStdLib;
    using bot::pos;
    static SListConstructor S;

    PlgTerm position = args.Car();
    PlgReference result = args.Cdr().Car();

    GameField field = position->Args().Car();
    SReference color = position->Args().Cdr().Car();
    SReference history = position->Args().Cdr().Cdr().Car();

    SReference list = field->CorrectMoves(color.GetInt());

    if (list.IsEmptyList())
        return false;

    SReference positionList = *PTheEmptyList;
    SReference oppColor = OpponentColor(color.GetInt());
    for (SReference p = list; !p.IsEmptyList(); p = p.Cdr()) {
        SReference move = p.Car();
        SReference newHistory = move.Car() ^ history;
        SReference newField = move.Cdr();
        positionList = pos(newField, oppColor, newHistory) ^ positionList;
    }

    return result.Unify(positionList, cont.Context());
}

// opponent_color(+Color,-Result)
static bool predicateOpponentColor(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    PlgReference color = args.Car();
    PlgReference result = args.Cdr().Car();

    int oc = OpponentColor(color.GetInt());
    result.Unify(SReference(oc), cont.Context());
}

// point_row(+Point,-Row)
static bool predicatePointRow(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    Point point = args.Car();
    PlgReference result = args.Cdr().Car();
    return result.Unify(SReference(point->Row()), cont.Context());
}

// point_col(+Point,-Col)
static bool predicatePointCol(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    Point point = args.Car();
    PlgReference result = args.Cdr().Car();
    return result.Unify(SReference(point->Col()), cont.Context());
}

// score(+Field,+Color,-Score)
static bool predicateScore(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    GameField field = args.Car();
    int color = args.Cdr().Car().GetInt();
    PlgReference result = args.Cdr().Cdr().Car();

    int sum = 0;
    for (int i = 0; i < field->Rows(); ++i)
        for (int j = 0; j < field->Cols(); ++j) {
            int chip = field->Get(i,j);
            if (chip == color)
                ++sum;
            else if (chip == OpponentColor(color))
                --sum;
        }

    return result.Unify(SReference(sum), cont.Context());
}

PlgBot::PlgBot(int color) : Player(color)
{
    using namespace bot;

    InitDatabase(db);

    correct_moves.SetPredicate(predicateCorrectMoves, 2);
    score.SetPredicate(predicateScore, 3);
    point_row.SetPredicate(predicatePointRow, 2);
    point_col.SetPredicate(predicatePointCol, 2);
}

void PlgBot::Move(const GameField &field, MoveCallback callback)
{
    using namespace bot;
    using namespace PlgStdLib;

    PlgVariable Row("Row"), Col("Col");
    PlgContinuation cont = db.Query( move(field, SReference(Color()), Row, Col) );
    debug("running query\n");
    bool status = cont->Next();
    assert(status);
    
    SReference row = cont->GetValue(Row),
               col = cont->GetValue(Col);

    debug("callback\n");
    callback(row.GetInt(), col.GetInt());
}
