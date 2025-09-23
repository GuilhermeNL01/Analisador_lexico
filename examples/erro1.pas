program erro1;
var
   x : integer;

begin
   x := 10;
   @x := 5;   { caractere inválido '@' }
end.