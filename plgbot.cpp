#include "plgbot.hpp"
#include "reversi.hpp"
#include "bot.hpp"
#include <prolog/prolog.hpp>
#include <prolog/library/library.hpp>
#include <cassert>
#include <climits>

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

// int_min(-Result)
static bool predicateIntMin(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    PlgReference result = args.Car();
    result.Unify(SReference(INT_MIN), cont.Context());
}

// int_max(-Result)
static bool predicateIntMax(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    PlgReference result = args.Car();
    result.Unify(SReference(INT_MAX), cont.Context());
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

static const int Size = 8;

static int startTable[Size][Size];
static int finalTable[Size][Size];

// score(+Field,+Color,-Score)
static bool predicateScore(const PlgAtom &functor, const SReference &args, PlgExpressionContinuation &cont)
{
    bool initDone = false;

    if (!initDone) {
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < Size; ++j) {
                finalTable[i][j] = 1;

                bool hor = (i == 0) || (i == Size-1);
                bool ver = (j == 0) || (j == Size-1);

                if (hor && ver)
                    startTable[i][j] = 25;
                else if (hor || ver)
                    startTable[i][j] = 5;
                else
                    startTable[i][j] = 1;
            }
        }

        initDone = true;
    }

    GameField field = args.Car();
    int color = args.Cdr().Car().GetInt();
    PlgReference result = args.Cdr().Cdr().Car();

    int empty = field->Score(CHIP_NONE);

    int (*table)[Size][Size];
    if (empty > 5)
        table = &startTable;
    else
        table = &finalTable;

    int sum = 0;
    for (int i = 0; i < field->Rows(); ++i)
        for (int j = 0; j < field->Cols(); ++j) {
            int chip = field->Get(i,j);
            int factor = 0;
            int score = 1;

            if (chip == color)
                factor = 1;
            else if (chip == OpponentColor(color))
                factor = -1;

            sum += factor * (*table)[i][j];
        }

    return result.Unify(SReference(sum), cont.Context());
}

PlgBot::PlgBot(int color) : Player(color)
{
    using namespace bot;

    bot::Init();

    correct_moves.SetPredicate(predicateCorrectMoves, 2);
    score.SetPredicate(predicateScore, 3);
    point_row.SetPredicate(predicatePointRow, 2);
    point_col.SetPredicate(predicatePointCol, 2);
    int_min.SetPredicate(predicateIntMin, 1);
    int_max.SetPredicate(predicateIntMax, 1);
}

void PlgBot::Move(const GameField &field, MoveCallback callback)
{
    using namespace bot;
    using namespace PlgStdLib;

    PlgVariable Row("Row"), Col("Col");
    PlgContinuation cont = move(field, SReference(Color()), Row, Col).Query();
    debug("running query");
    bool status = cont->Next();
    assert(status);
    
    SReference row = cont->GetValue(Row),
               col = cont->GetValue(Col);

    debug("callback");
    callback(row.GetInt(), col.GetInt());
}
