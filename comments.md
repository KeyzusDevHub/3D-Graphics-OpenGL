# Total 132/147

# House 2/2

16.10.2023 OK

# Colors 5/5

16.10.2023 OK

# Indices 5/5

16.10.2023 OK

# Uniforms 8/8

23.10.2023 12:37:44 OK

# PVM 5/5

23.10.2023 12:38:49 OK
Ale skoro już Państwo tworzą macierz M to przesunięcie powinno być w niej zawarte. 

# Pyramid 4/4

23.10.2023 12:40:43 OK


# Resize 8/8

30.10.2023 12:31:03 
Przesyłanie macierzy PVM, powinno mieć miejsce przed wywołaniem polecenia drawElements. W tej sytuacji w pierwszek klatce macierz perspective jest zła.
Poza tym OK

13.11.2023 13:45:06 OK

# Zoom 10/10

13.11.2023 13:46:16
Znów zła kolejność we `frame`. 

20.11.2023 18:43:32 OK

# Camera movement 15/15

13.11.2023 13:47:28
Znów zła kolejnoś we `frame`. 

20.11.2023 18:44:01 OK

# Mesh 10/10

29.11.2023 10:25:41 OK

# Kd material 20/20

4.12.2023 14:53:25
Brakuje konstruktora/metody który umożliwyłby ustawienie wartości pola `use_vertex_colors_`. 

18.12.2023 11:39:17
Jeśli `use_vertex_colors` ma wartość `true` kolor pixela powinien być iloczynem `vertex_color` i `Kd`.

8.01.2024 17:10:42 OK

# Textures 15/15

18.12.2023 11:40:48
Podobnie jak w poprzednim ćwiczeniu, ale jeszcze dodatkowo jeśli tekstura jest obecna to jej kolor mnoży `vertex_color` i Kd. 

8.01.2024 17:12:09 OK

# OBJ reader 5/5

8.01.2024 17:13:04 OK

# Diffuse 20/25

16.01.2024 14:46:39
Co ma oznaczać ten żółty kwadrat?

22.01.2024 16:06:19 OK
Ka jest nieużywane w szaderze. 

8.01.2024 13:18:01
Żle Panowie liczą kolory w szaderze.
color = Ka*ambient+color_Kd*diffuse + Ks*specular.
gdzie ambiet, specular i diffuse to czlonyw zwizane wyłącznie z oświetleniem. Te czlony maja Panowie poprawnie, ale calość nie jest dobrze poskladana. 
color_Kd = vertex_color * Kd * texture_color; 

30.01.2024 10:38:51 OK

# Specular

22.01.2024 16:08:28
Jak ustalana jest wartość `Ns`?

30.01.2024 10:39:19
Do liczenia specular uzywają Panowie dot(V,H) anie dot(N,H). Ponadto w czesci specular brakuje attenutaion, koloru i natężenia światła. 
