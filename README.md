# Philosophers
Eating philosophers problem solution with C++


1. Uruchomienie:

   Aby zapewnić poprawne uruchomienie, należy w konsoli, tuż po nazwie pliku, podać dokładnie jeden argument
   będący liczbą całkowitą, oznaczający pożądaną liczbą filozofów.

3. Wątki:

    Liczba wątków jest zmienna, równa wartości przekazanej jako argument podczas uruchomienia programu, jako że
    każdemu symulowanemu filozofowi odpowiada dokładnie jeden wątek.

4. Sekcje krytyczne:

    1. Związane z dostępem do wektora stanów filozofów - występuje w funkcjach put_forks, take_forks oraz try_to_eat
    2. Związane z wyświetleniem tekstu w terminalu - występuje w funkcji print_states
  
     Oba rodzaje sekcji krytycznych są rozwiązywane z wykorzystaniem mutexa, jednak są to dwa różne muteksy, osobne
     dla obu rodzajów sekcji krytycznych, co ma na celu zmniejszenie czasu potencjalnego oczekiwania wątku. Mutexy są
     zarządzane przez unique_lock, co ułatwia zarządzanie nimi, tj. blokowanie i odblokowywanie (co dzieje się
     automatycznie), jednak nie odbiera również możliwości manualnego sterowania nimi.
