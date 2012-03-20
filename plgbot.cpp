#include "plgbot.hpp"
#include "reversi.hpp"
#include "bot.hpp"
#include "prolog/prolog.hpp"
#include "prolog/library/library.hpp"
#include <cassert>

static bool predicateCorrectMoves(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    using namespace PlgStdLib;
    static SListConstructor S;

    GameField field = args.Car();
    SReference color = args.Cdr().Car();
    PlgReference result = args.Cdr().Cdr().Car();

    SReference list = field->CorrectMoves(color.GetInt());
    return result.Unify(list, cont.Context());
}

static bool predicatePointRow(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    Point point = args.Car();
    PlgReference result = args.Cdr().Car();
    return result.Unify(SReference(point->Row()), cont.Context());
}

static bool predicatePointCol(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    Point point = args.Car();
    PlgReference result = args.Cdr().Car();
    return result.Unify(SReference(point->Col()), cont.Context());
}

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

    correct_moves.SetPredicate(predicateCorrectMoves, 3);
    point_row.SetPredicate(predicatePointRow, 2);
    point_col.SetPredicate(predicatePointCol, 2);
}

void PlgBot::Move(const GameField &field, MoveCallback callback)
{
    using namespace bot;
    using namespace PlgStdLib;

    PlgVariable Row("Row"), Col("Col");
    PlgContinuation cont = Database().Query( move(field, SReference(Color()), Row, Col) );
    debug("running query\n");
    bool status = cont->Next();
    assert(status);
    
    SReference row = cont->GetValue(Row),
               col = cont->GetValue(Col);

    debug("callback\n");
    callback(row.GetInt(), col.GetInt());
}
