% Лабораторная работа №2 по дисциплине Логические Основы Интеллектуальных Систем
% Выполнена студентом группы 421701 БГУИР Аникеенко Андреем Дмитриевичем
%
% Задание:
% Два берега реки. На одном из них человек, который несёт капусту, ведёт козу и пойманного волка. 
% Требуется с помощью лодки, вмещающей вместе с человеком не более одного животного или предмета, переправиться на другой берег.
% Человек не может оставлять козу с капустой и волка с козой.
%
% Использованные источники:
% Zuse, K. (Конрад Цузе) Der Plankalkül [плановое исчисление] / K. Zuse // Gesellschaft für Mathematik und Datenverarbeitung
% [Общество математики и обработки данных]. – 1972.
% SWI-Prolog Documentation: predicate member/2 [Электронный ресурс]. – Режим доступа: https://www.swi-prolog.org/pldoc/man?predicate=member/2.
% SWI-Prolog Documentation: Built-in Predicates (append, findall, msort, reverse) [Электронный ресурс].
% – Режим доступа: https://www.swi-prolog.org/pldoc/man?section=builtin.

% --- Безопасность берега ---

safe(Bank) :-
    ( member(man, Bank) -> true
    ;
        \+ (member(wolf, Bank), member(goat, Bank)),
        \+ (member(goat, Bank), member(cabbage, Bank))
    ).

% --- Переходы ---

move(state(Left, Right, left), state(NewLeft, NewRight, right), Carry) :-
    member(man, Left),
    ( Carry = none ; (member(Carry, Left), Carry \= man) ),
    ( Carry = none -> remove(man, Left, NewLeft)
    ; remove_list([man, Carry], Left, NewLeft) ),
    ( Carry = none -> NewRight = [man | Right]
    ; NewRight = [man, Carry | Right] ),
    safe(NewLeft),
    safe(NewRight).

move(state(Left, Right, right), state(NewLeft, NewRight, left), Carry) :-
    member(man, Right),
    ( Carry = none ; (member(Carry, Right), Carry \= man) ),
    ( Carry = none -> remove(man, Right, NewRight)
    ; remove_list([man, Carry], Right, NewRight) ),
    ( Carry = none -> NewLeft = [man | Left]
    ; NewLeft = [man, Carry | Left] ),
    safe(NewLeft),
    safe(NewRight).

% --- Вспомогательные ---

remove(_, [], []).
remove(X, [X|T], T) :- !.
remove(X, [H|T], [H|R]) :- remove(X, T, R).

remove_list([], L, L).
remove_list([H|T], L, R) :-
    remove(H, L, L1),
    remove_list(T, L1, R).

normalized(state(L, R, B), state(SL, SR, B)) :-
    msort(L, SL), msort(R, SR).

% --- BFS ---

solve(InitState, GoalState, Solution) :-
    normalized(InitState, NInit),
    normalized(GoalState, NGoal),
    bfs([[NInit]], NGoal, RevPath),
    reverse(RevPath, Solution).

bfs([[GoalNorm | Rest] | _], GoalNorm, [GoalNorm | Rest]) :- !.
bfs([[Current | Visited] | Queue], Goal, Solution) :-
    findall(
        [Next, Current | Visited],
        (
            move(Current, NextRaw, _),
            normalized(NextRaw, Next),
            \+ member(Next, [Current | Visited])
        ),
        NewPaths
    ),
    append(Queue, NewPaths, NewQueue),
    bfs(NewQueue, Goal, Solution).

% --- Печать ---

print_solution([]) :- nl.
print_solution([state(L, R, B) | Rest]) :-
    msort(L, SL), msort(R, SR),
    write('  Левый берег : '), write(SL),
    write('  |  Лодка: '), write(B),
    write('  |  Правый берег: '), write(SR), nl,
    print_solution(Rest).

% --- Ввод строками ---

% Читаем строку и парсим её как Prolog-терм
read_term_from_line(Term) :-
    read_line_to_string(user_input, Line),
    % Добавляем точку если пользователь не поставил
    ( string_concat(_, ".", Line) ->
        TermStr = Line
    ;
        string_concat(Line, ".", TermStr)
    ),
    term_to_atom(Term, TermStr).

% Читаем yes/no
ask_yes_no(Question, Answer) :-
    repeat,
        format("~w (yes/no): ", [Question]),
        read_line_to_string(user_input, Line),
        string_lower(Line, Low),
        ( Low = "yes" -> Answer = yes, !
        ; Low = "no"  -> Answer = no,  !
        ;
            write('  Введите yes или no'), nl,
            fail
        ).

% Читаем список объектов
% Допустимые: man, wolf, goat, cabbage
all_objects([man, wolf, goat, cabbage]).

read_bank(Prompt, List) :-
    repeat,
        format("~w~n", [Prompt]),
        write('  Допустимые объекты: man wolf goat cabbage'), nl,
        write('  Введите через пробел (или оставьте пустым для пустого берега): '),
        read_line_to_string(user_input, Line),
        split_string(Line, " ", " ", Parts),
        include([P]>>(P \= ""), Parts, NonEmpty),
        maplist(atom_string, Atoms, NonEmpty),
        all_objects(Allowed),
        ( maplist([A]>>(member(A, Allowed)), Atoms) ->
            List = Atoms, !
        ;
            write('  Ошибка: недопустимые объекты. Попробуйте снова.'), nl,
            fail
        ).

% Читаем сторону лодки
read_side(Prompt, Side) :-
    repeat,
        format("~w (left/right): ", [Prompt]),
        read_line_to_string(user_input, Line),
        string_lower(Line, Low),
        ( Low = "left"  -> Side = left,  !
        ; Low = "right" -> Side = right, !
        ;
            write('  Ошибка: введите left или right'), nl,
            fail
        ).

% Валидация: man ровно на одном берегу
validate_man(Left, Right) :-
    ( member(man, Left), \+ member(man, Right) -> true
    ; member(man, Right), \+ member(man, Left) -> true
    ;
        write('  Ошибка: man должен быть ровно на одном берегу.'), nl,
        fail
    ).

% Валидация: нет дублей между берегами
no_duplicates(Left, Right) :-
    ( \+ (member(X, Left), member(X, Right)) -> true
    ;
        write('  Ошибка: один объект не может быть на обоих берегах.'), nl,
        fail
    ).

% Читаем полное состояние
read_state(Label, state(Left, Right, Boat)) :-
    repeat,
        format("~n=== ~w ===~n", [Label]),
        read_bank('Левый берег:', Left),
        read_bank('Правый берег:', Right),
        read_side('Сторона лодки', Boat),
        ( validate_man(Left, Right), no_duplicates(Left, Right) ->
            !
        ;
            write('  Некорректное состояние, повторите ввод.'), nl,
            fail
        ).

% --- Главный предикат ---

main :-
    write('Ограничения:'), nl,
    write('  - Лодка вмещает человека + не более 1 объекта'), nl,
    write('  - Нельзя оставлять волка с козой без человека'), nl,
    write('  - Нельзя оставлять козу с капустой без человека'), nl,
    nl,
    ask_yes_no('Использовать стандартное начальное состояние?', UseDefault),
    nl,
    ( UseDefault = yes ->
        Init = state([man, wolf, goat, cabbage], [], left),
        Goal = state([], [man, wolf, goat, cabbage], right),
        write('Начальное: все на левом берегу.'), nl,
        write('Целевое:   все на правом берегу.'), nl
    ;
        read_state('НАЧАЛЬНОЕ СОСТОЯНИЕ', Init),
        read_state('ЦЕЛЕВОЕ СОСТОЯНИЕ',   Goal)
    ),
    nl,
    write('Поиск решения...'), nl, nl,
    ( solve(Init, Goal, Solution) ->
        length(Solution, Len),
        Steps is Len - 1,
        format("Решение найдено за ~w шагов:~n~n", [Steps]),
        print_solution(Solution)
    ;
        write('Решение не найдено для заданных состояний.'), nl
    ).

 :- initialization(main, main).
