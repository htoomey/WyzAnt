{
NAME: Harold Toomey
TASK: USACO Automated Test Template
LANG: PASCAL
}

Program USACO;
Var fin, fout: text;
    a, b: word;
Begin
    Assign(fin, 'test.in'); Reset(fin);
    Assign(fout, 'test.out'); Rewrite(fout);
    Readln(fin, a, b);
    Writeln(fout, a + b);
    Close(fout);
End.
