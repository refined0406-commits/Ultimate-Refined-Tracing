# -*- coding: utf-8 -*-
"""V5.3.0 PRIVATE rev2: description cores (18 languages).
 * MasterEnabled: the owner's addition about restart-dependent settings; the immediate-restoration wording is
   qualified ("for most settings") so Master Off never promises an instant change for the restart rows.
 * URTX_RTAsyncAO / URTX_RTAsyncGIShading: the word "Experimental" removed (owner's request); the last sentence
   now says the result depends on the GPU. Everything else is the gameplay-approved V5.3.0 text, unchanged.
 * Three new controls (Native / Auto by default, no Quick Preset): async GI probe classification, async
   acceleration-structure compaction, animated-geometry trace mode."""
import re
from tip_core_d import CORE as _D

ASYNC_TAIL = {
 'en': "Depending on your GPU, it can be faster or slower.",
 'de': "Je nach GPU kann es schneller oder langsamer sein.",
 'fr': "Selon votre GPU, cela peut être plus rapide ou plus lent.",
 'es': "Según la GPU, puede ser más rápido o lento.",
 'esmx': "Según la GPU, puede ser más rápido o lento.",
 'it': "In base alla GPU, può essere più veloce o lento.",
 'pl': "Zależnie od GPU może być szybciej lub wolniej.",
 'ru': "В зависимости от GPU может быть быстрее или медленнее.",
 'ua': "Залежно від GPU може бути швидше або повільніше.",
 'cz': "Podle GPU může být rychlejší i pomalejší.",
 'hu': "A GPU-tól függően gyorsabb vagy lassabb is lehet.",
 'tr': "GPU'ya göre daha hızlı ya da yavaş olabilir.",
 'br': "Dependendo da sua GPU, pode ser mais rápido ou mais lento.",
 'jp': "GPU によって速くも遅くもなり得ます。",
 'kr': "GPU에 따라 빨라지거나 느려질 수 있습니다.",
 'cn': "视 GPU 而定，可能更快也可能更慢。",
 'zh': "視 GPU 而定，可能更快也可能更慢。",
 'ar': "حسب وحدة GPU لديك، قد يكون أسرع أو أبطأ.",
}

def _no_experimental(text, lang):
    """Drop the first sentence ("Experimental.") and replace the last one ("It can be faster or slower.")."""
    parts = [p for p in re.split(r'(?<=[.。])\s*', text.strip()) if p]
    body = parts[1:-1]
    sep = '' if lang in ('jp', 'cn', 'zh') else ' '
    return sep.join(body + [ASYNC_TAIL[lang]])

CORE = {}
for _k in ('URTX_RTAsyncAO', 'URTX_RTAsyncGIShading'):
    CORE[_k] = {l: _no_experimental(t, l) for l, t in _D[_k].items()}

CORE['MasterEnabled'] = {
 'en': "Turns every Ultimate Refined Tracing change on or off. Off restores the game's own values, at once for most settings, and keeps your choices for later; it never changes the game's RT/PT mode. Some settings require a game restart to take effect. Check each setting's description for details. FPS effects in these tooltips are expectations and vary by scene and hardware.",
 'de': "Schaltet alle Änderungen von Ultimate Refined Tracing ein oder aus. Aus stellt die Werte des Spiels wieder her, bei den meisten Einstellungen sofort, und behält deine Auswahl; der RT/PT-Modus wird nie geändert. Manche Einstellungen erfordern einen Neustart des Spiels. Details stehen in der Beschreibung jeder Einstellung. FPS-Angaben sind Erwartungen und hängen von Szene und Hardware ab.",
 'fr': "Active ou désactive toutes les modifications d'Ultimate Refined Tracing. Désactivé rétablit les valeurs du jeu, aussitôt pour la plupart des réglages, et conserve vos choix ; le mode RT/PT n'est jamais modifié. Certains réglages nécessitent un redémarrage du jeu. Voir la description de chaque réglage. Les effets sur les FPS sont des attentes et varient selon la scène et le matériel.",
 'es': "Activa o desactiva todos los cambios de Ultimate Refined Tracing. Desactivado restaura los valores del juego, al momento en la mayoría de ajustes, y conserva tus elecciones; nunca cambia el modo RT/PT. Algunos ajustes requieren reiniciar el juego. Consulta la descripción de cada ajuste. Los efectos en FPS son estimaciones y varían según la escena y el hardware.",
 'esmx': "Activa o desactiva todos los cambios de Ultimate Refined Tracing. Desactivado restaura los valores del juego, al momento en la mayoría de ajustes, y conserva tus elecciones; nunca cambia el modo RT/PT. Algunos ajustes requieren reiniciar el juego. Consulta la descripción de cada ajuste. Los efectos en FPS son estimaciones y varían según la escena y el hardware.",
 'it': "Attiva o disattiva tutte le modifiche di Ultimate Refined Tracing. Disattivato ripristina i valori del gioco, subito per la maggior parte delle impostazioni, e conserva le tue scelte; non cambia mai la modalità RT/PT. Alcune impostazioni richiedono il riavvio del gioco. Vedi la descrizione di ogni impostazione. Gli effetti sugli FPS sono stime e variano con scena e hardware.",
 'pl': "Włącza lub wyłącza wszystkie zmiany Ultimate Refined Tracing. Wyłączone przywraca wartości gry, w większości ustawień od razu, i zachowuje twoje wybory; nigdy nie zmienia trybu RT/PT. Niektóre ustawienia wymagają ponownego uruchomienia gry. Szczegóły w opisie każdego ustawienia. Wpływ na FPS to oczekiwania, zależne od sceny i sprzętu.",
 'ru': "Включает или выключает все изменения Ultimate Refined Tracing. Выкл. возвращает значения игры, для большинства настроек сразу, и сохраняет ваш выбор; режим RT/PT не меняется. Некоторые настройки требуют перезапуска игры. Подробности — в описании каждой настройки. Влияние на FPS — ожидаемое и зависит от сцены и железа.",
 'ua': "Вмикає або вимикає всі зміни Ultimate Refined Tracing. Вимк. повертає значення гри, для більшості налаштувань одразу, і зберігає ваш вибір; режим RT/PT не змінюється. Деякі налаштування потребують перезапуску гри. Подробиці — в описі кожного налаштування. Вплив на FPS — очікуваний і залежить від сцени та заліза.",
 'cz': "Zapíná nebo vypíná všechny změny Ultimate Refined Tracing. Vypnuto obnoví hodnoty hry, u většiny nastavení okamžitě, a ponechá vaši volbu; režim RT/PT se nikdy nemění. Některá nastavení vyžadují restart hry. Podrobnosti najdete v popisu každého nastavení. Vliv na FPS je očekávaný a závisí na scéně a hardwaru.",
 'hu': "Az Ultimate Refined Tracing összes módosítását be- vagy kikapcsolja. A Ki visszaállítja a játék értékeit, a legtöbb beállításnál azonnal, és megtartja a választásaidat; az RT/PT módot soha nem változtatja. Egyes beállításokhoz a játék újraindítása szükséges. A részletek az egyes beállítások leírásában. Az FPS-hatások várható értékek, jelenettől és hardvertől függnek.",
 'tr': "Ultimate Refined Tracing'in tüm değişikliklerini açar veya kapatır. Kapalı, oyunun değerlerini çoğu ayarda hemen geri yükler ve seçimlerinizi saklar; RT/PT modunu asla değiştirmez. Bazı ayarlar oyunun yeniden başlatılmasını gerektirir. Ayrıntılar her ayarın açıklamasında. FPS etkileri beklentidir; sahneye ve donanıma göre değişir.",
 'br': "Liga ou desliga todas as mudanças do Ultimate Refined Tracing. Desligado restaura os valores do jogo, na hora para a maioria das opções, e guarda suas escolhas; nunca muda o modo RT/PT. Algumas opções exigem reiniciar o jogo. Veja a descrição de cada opção. Os efeitos de FPS são estimativas e variam conforme a cena e o hardware.",
 'jp': "Ultimate Refined Tracing のすべての変更をオン／オフします。オフにするとゲーム本来の値に戻り（ほとんどの設定は即時）、選択内容は保持されます。RT/PT モードは変更しません。一部の設定は反映にゲームの再起動が必要です。詳しくは各設定の説明をご覧ください。FPS への効果は見込みであり、シーンやハードウェアで変わります。",
 'kr': "Ultimate Refined Tracing의 모든 변경을 켜거나 끕니다. 끄기는 게임 값으로 되돌리며(대부분의 설정은 즉시) 선택은 저장해 둡니다. RT/PT 모드는 바꾸지 않습니다. 일부 설정은 적용하려면 게임을 다시 시작해야 합니다. 자세한 내용은 각 설정의 설명을 확인하세요. FPS 효과는 예상치이며 장면과 하드웨어에 따라 다릅니다.",
 'cn': "开启或关闭 Ultimate Refined Tracing 的全部更改。关闭会恢复游戏自身的数值（大多数设置立即生效）并保留你的选择；不会改变 RT/PT 模式。部分设置需要重启游戏才能生效。详情请查看各设置的说明。FPS 效果均为预期，随场景和硬件而变化。",
 'zh': "開啟或關閉 Ultimate Refined Tracing 的全部變更。關閉會恢復遊戲自身的數值（大多數設定立即生效）並保留你的選擇；不會改變 RT/PT 模式。部分設定需要重新啟動遊戲才會生效。詳情請參閱各設定的說明。FPS 效果均為預期，隨場景與硬體而變化。",
 'ar': "يشغل أو يوقف كل تغييرات Ultimate Refined Tracing. الإيقاف يعيد قيم اللعبة، فورًا لمعظم الإعدادات، ويحتفظ باختياراتك؛ ولا يغير وضع RT/PT أبدًا. بعض الإعدادات تتطلب إعادة تشغيل اللعبة لتسري. راجع وصف كل إعداد للتفاصيل. تأثيرات FPS متوقعة وتختلف حسب المشهد والعتاد.",
}

CORE['URTX_RTAsyncGIClassify'] = {
 'en': "Runs the ray-traced GI probe classification on the GPU's async compute queue so it can overlap other rendering. This is the game's own path, off by default; the image is the same. " + ASYNC_TAIL['en'],
 'de': "Berechnet die Klassifizierung der Raytracing-GI-Sonden in der Async-Compute-Warteschlange der GPU, damit sie sich mit anderer Arbeit überlappt. Ein eigener Pfad des Spiels, standardmäßig aus; das Bild bleibt gleich. " + ASYNC_TAIL['de'],
 'fr': "Exécute la classification des sondes GI en ray tracing sur la file de calcul asynchrone du GPU pour la superposer au reste du rendu. C'est un chemin du jeu, désactivé par défaut ; l'image est identique. " + ASYNC_TAIL['fr'],
 'es': "Ejecuta la clasificación de las sondas GI con ray tracing en la cola de cómputo asíncrono de la GPU para solaparla con el resto. Es una ruta del juego, apagada por defecto; la imagen es igual. " + ASYNC_TAIL['es'],
 'esmx': "Ejecuta la clasificación de las sondas GI con ray tracing en la cola de cómputo asíncrono de la GPU para traslaparla con el resto. Es una ruta del juego, apagada por defecto; la imagen es igual. " + ASYNC_TAIL['esmx'],
 'it': "Esegue la classificazione delle sonde GI in ray tracing sulla coda di calcolo asincrono della GPU, sovrapponendola al resto. È un percorso del gioco, disattivato di default; l'immagine non cambia. " + ASYNC_TAIL['it'],
 'pl': "Liczy klasyfikację sond GI z ray tracingiem w asynchronicznej kolejce obliczeń GPU, by nakładała się na resztę renderowania. To ścieżka samej gry, domyślnie wyłączona; obraz jest taki sam. " + ASYNC_TAIL['pl'],
 'ru': "Выполняет классификацию трассированных зондов GI в асинхронной очереди вычислений GPU, параллельно с остальным рендерингом. Это путь самой игры, по умолчанию выкл.; изображение то же. " + ASYNC_TAIL['ru'],
 'ua': "Виконує класифікацію трасованих зондів GI в асинхронній черзі обчислень GPU, щоб вона йшла паралельно з рештою рендерингу. Це власний шлях гри, вимкнений за замовчуванням; зображення те саме. " + ASYNC_TAIL['ua'],
 'cz': "Počítá klasifikaci sond GI s ray tracingem v asynchronní výpočetní frontě GPU, aby se překrývala s ostatním vykreslováním. Je to cesta samotné hry, ve výchozím stavu vypnutá; obraz je stejný. " + ASYNC_TAIL['cz'],
 'hu': "A sugárkövetett GI-szondák osztályozását a GPU aszinkron számítási sorában futtatja, hogy átfedjen a többi rendereléssel. A játék saját útja, alapból kikapcsolva; a kép ugyanaz. " + ASYNC_TAIL['hu'],
 'tr': "Işın izlemeli GI sondalarının sınıflandırmasını GPU'nun asenkron hesaplama kuyruğunda çalıştırır, böylece diğer işlemeyle örtüşebilir. Oyunun kendi yoludur, varsayılan olarak kapalıdır; görüntü aynıdır. " + ASYNC_TAIL['tr'],
 'br': "Executa a classificação das sondas GI com ray tracing na fila de computação assíncrona da GPU para sobrepor ao restante. É um caminho do próprio jogo, desligado por padrão; a imagem é a mesma. " + ASYNC_TAIL['br'],
 'jp': "レイトレーシング GI プローブの分類を GPU の非同期コンピュートキューで実行し、他の描画と重ねます。ゲーム本来の処理（既定でオフ）で、画質は同じです。" + ASYNC_TAIL['jp'],
 'kr': "레이 트레이싱 GI 프로브 분류를 GPU의 비동기 컴퓨트 큐에서 실행해 다른 렌더링과 겹치게 합니다. 게임 자체 경로이며 기본값은 꺼짐이고 화면은 같습니다. " + ASYNC_TAIL['kr'],
 'cn': "将光线追踪全局光照探针的分类放到 GPU 的异步计算队列中执行，使其与其他渲染重叠。这是游戏自带的路径，默认关闭；画面相同。" + ASYNC_TAIL['cn'],
 'zh': "將光線追蹤全域光照探針的分類放到 GPU 的非同步運算佇列執行，使其與其他繪製重疊。這是遊戲內建的路徑，預設關閉；畫面相同。" + ASYNC_TAIL['zh'],
 'ar': "يشغّل تصنيف مجسات الإضاءة الشاملة بتتبع الأشعة على طابور الحوسبة غير المتزامنة في GPU ليتداخل مع بقية العرض. إنه مسار اللعبة نفسها، معطل افتراضيًا؛ الصورة نفسها. " + ASYNC_TAIL['ar'],
}

# R5: async acceleration-structure compaction removed (crashed in the owner's test); its description is gone too.

CORE['URTX_RTASFastTrace'] = {
 'en': "How the game builds ray-tracing structures for animated geometry (characters, swaying trees). Fast Trace makes rays through them cheaper, but rebuilding them each frame costs more; the image is the same. Whether it is faster overall depends on your GPU and the scene.",
 'de': "Wie das Spiel Raytracing-Strukturen für animierte Geometrie (Figuren, schwankende Bäume) baut. Schnelles Tracing macht Strahlen durch sie günstiger, ihr Neuaufbau pro Frame kostet aber mehr; das Bild bleibt gleich. Ob es insgesamt schneller ist, hängt von GPU und Szene ab.",
 'fr': "Comment le jeu construit les structures de ray tracing pour la géométrie animée (personnages, arbres qui ondulent). Le tracé rapide rend les rayons moins coûteux, mais les reconstruire à chaque image coûte plus ; l'image est identique. Le gain global dépend du GPU et de la scène.",
 'es': "Cómo construye el juego las estructuras de ray tracing para la geometría animada (personajes, árboles que se mecen). El trazado rápido abarata los rayos, pero reconstruirlas en cada fotograma cuesta más; la imagen es igual. Que sea más rápido en total depende de la GPU y la escena.",
 'esmx': "Cómo construye el juego las estructuras de ray tracing para la geometría animada (personajes, árboles que se mecen). El trazado rápido abarata los rayos, pero reconstruirlas en cada cuadro cuesta más; la imagen es igual. Que sea más rápido en total depende de la GPU y la escena.",
 'it': "Come il gioco costruisce le strutture di ray tracing per la geometria animata (personaggi, alberi che ondeggiano). Il tracciamento rapido rende i raggi più economici, ma ricostruirle a ogni fotogramma costa di più; l'immagine non cambia. Se sia più veloce nel complesso dipende da GPU e scena.",
 'pl': "Jak gra buduje struktury ray tracingu dla animowanej geometrii (postacie, kołyszące się drzewa). Szybkie śledzenie tanieje dla promieni, ale przebudowa w każdej klatce kosztuje więcej; obraz jest taki sam. Czy całość będzie szybsza, zależy od GPU i sceny.",
 'ru': "Как игра строит структуры трассировки для анимированной геометрии (персонажи, качающиеся деревья). Быстрая трассировка удешевляет лучи, но перестройка каждый кадр дороже; изображение то же. Будет ли быстрее в целом, зависит от GPU и сцены.",
 'ua': "Як гра будує структури трасування для анімованої геометрії (персонажі, дерева, що гойдаються). Швидке трасування здешевлює промені, але перебудова щокадру дорожча; зображення те саме. Чи буде швидше загалом, залежить від GPU і сцени.",
 'cz': "Jak hra staví struktury ray tracingu pro animovanou geometrii (postavy, kývající se stromy). Rychlé trasování zlevní paprsky, ale přestavba v každém snímku stojí víc; obraz je stejný. Zda to bude celkově rychlejší, závisí na GPU a scéně.",
 'hu': "Hogyan építi a játék a sugárkövetési struktúrákat az animált geometriához (szereplők, hajladozó fák). A gyors követés olcsóbbá teszi a sugarakat, de a képkockánkénti újraépítés többe kerül; a kép ugyanaz. Hogy összességében gyorsabb-e, a GPU-tól és a jelenettől függ.",
 'tr': "Oyunun animasyonlu geometri (karakterler, sallanan ağaçlar) için ışın izleme yapılarını nasıl kurduğu. Hızlı izleme ışınları ucuzlatır, ancak her karede yeniden kurmak daha pahalıdır; görüntü aynıdır. Genelde daha hızlı olup olmadığı GPU'ya ve sahneye bağlıdır.",
 'br': "Como o jogo constrói as estruturas de ray tracing da geometria animada (personagens, árvores balançando). O traçado rápido barateia os raios, mas reconstruí-las a cada quadro custa mais; a imagem é a mesma. Se fica mais rápido no total depende da GPU e da cena.",
 'jp': "アニメーションするジオメトリ（キャラクター、揺れる木）のレイトレーシング構造の作り方です。高速トレースはレイが安くなりますが、毎フレームの再構築は重くなります。画質は同じです。全体で速くなるかは GPU とシーンによります。",
 'kr': "애니메이션 지오메트리(캐릭터, 흔들리는 나무)의 레이 트레이싱 구조를 만드는 방식입니다. 빠른 추적은 광선 비용을 줄이지만 매 프레임 재구성 비용은 늘어납니다. 화면은 같습니다. 전체적으로 빨라지는지는 GPU와 장면에 따라 다릅니다.",
 'cn': "游戏为动画几何体（角色、摇摆的树）构建光线追踪结构的方式。快速追踪让光线更省，但每帧重建的开销更大；画面相同。整体是否更快取决于 GPU 和场景。",
 'zh': "遊戲為動畫幾何體（角色、搖擺的樹）建構光線追蹤結構的方式。快速追蹤讓光線更省，但每幀重建的開銷更大；畫面相同。整體是否更快取決於 GPU 與場景。",
 'ar': "طريقة بناء اللعبة لهياكل تتبع الأشعة للهندسة المتحركة (الشخصيات، الأشجار المتمايلة). التتبع السريع يجعل الأشعة أرخص، لكن إعادة بنائها كل إطار أكثر كلفة؛ الصورة نفسها. كونه أسرع إجمالًا يعتمد على GPU والمشهد.",
}

CORE['URTX_PTMoonLight'] = {
 'en': "Strength of the moon's direct light in Path Tracing. The game blends sun and moon light by time of day, so this changes nights only. Higher values give brighter, clearer nights; lower values darker, moodier ones. Daylight, torches and other lights are unchanged. Try 0.75–1.50 for subtle changes.",
 'de': "Stärke des direkten Mondlichts beim Path Tracing. Das Spiel mischt Sonnen- und Mondlicht nach Tageszeit, daher ändert dies nur die Nächte. Höhere Werte ergeben hellere, klarere Nächte, niedrigere dunklere, stimmungsvollere. Tageslicht, Fackeln und andere Lichter bleiben gleich. Für leichte Änderungen 0,75–1,50.",
 'fr': "Intensité de la lumière directe de la lune en Path Tracing. Le jeu mélange soleil et lune selon l'heure, donc seules les nuits changent. Plus élevé : nuits plus claires et lisibles ; plus bas : nuits plus sombres. Le jour, les torches et les autres lumières ne changent pas. Essayez 0,75–1,50 pour un effet léger.",
 'es': "Intensidad de la luz directa de la luna en Path Tracing. El juego mezcla la luz del sol y de la luna según la hora, así que solo cambian las noches. Valores altos dan noches más claras; bajos, más oscuras. El día, las antorchas y otras luces no cambian. Prueba 0,75–1,50 para cambios sutiles.",
 'esmx': "Intensidad de la luz directa de la luna en Path Tracing. El juego mezcla la luz del sol y de la luna según la hora, así que solo cambian las noches. Valores altos dan noches más claras; bajos, más oscuras. El día, las antorchas y otras luces no cambian. Prueba 0.75–1.50 para cambios sutiles.",
 'it': "Intensità della luce diretta della luna nel Path Tracing. Il gioco fonde luce del sole e della luna in base all'ora, quindi cambiano solo le notti. Valori alti danno notti più chiare; bassi, più scure. Giorno, torce e altre luci non cambiano. Prova 0,75–1,50 per cambi leggeri.",
 'pl': "Siła bezpośredniego światła księżyca w Path Tracingu. Gra miesza światło słońca i księżyca według pory dnia, więc zmieniają się tylko noce. Wyższe wartości dają jaśniejsze noce, niższe ciemniejsze. Dzień, pochodnie i inne światła bez zmian. Dla subtelnych zmian 0,75–1,50.",
 'ru': "Сила прямого лунного света в трассировке пути. Игра смешивает свет солнца и луны по времени суток, поэтому меняются только ночи. Больше — светлее и яснее ночи, меньше — темнее и мрачнее. День, факелы и другие источники не меняются. Для мягкого эффекта 0,75–1,50.",
 'ua': "Сила прямого місячного світла в трасуванні шляхів. Гра змішує світло сонця й місяця за часом доби, тож змінюються лише ночі. Більше — світліші ночі, менше — темніші. День, смолоскипи та інші джерела не змінюються. Для м'якого ефекту 0,75–1,50.",
 'cz': "Síla přímého měsíčního světla při Path Tracingu. Hra mísí světlo slunce a měsíce podle denní doby, takže se mění jen noci. Vyšší hodnoty dávají světlejší noci, nižší tmavší. Den, pochodně a jiná světla se nemění. Pro jemnou změnu 0,75–1,50.",
 'hu': "A hold közvetlen fényének erőssége Path Tracingben. A játék napszak szerint keveri a nap és a hold fényét, így csak az éjszakák változnak. Magasabb érték világosabb, alacsonyabb sötétebb éjszakát ad. A nappal, a fáklyák és más fények nem változnak. Finom változáshoz 0,75–1,50.",
 'tr': "Path Tracing'de ayın doğrudan ışığının gücü. Oyun güneş ve ay ışığını günün saatine göre karıştırır, bu yüzden yalnızca geceler değişir. Yüksek değerler daha aydınlık, düşükler daha karanlık geceler verir. Gündüz, meşaleler ve diğer ışıklar değişmez. Hafif etki için 0,75–1,50.",
 'br': "Intensidade da luz direta da lua no Path Tracing. O jogo mistura a luz do sol e da lua pela hora do dia, então só as noites mudam. Valores maiores dão noites mais claras; menores, mais escuras. Dia, tochas e outras luzes não mudam. Tente 0,75–1,50 para mudanças sutis.",
 'jp': "パストレーシングでの月の直接光の強さです。ゲームは時間帯に応じて太陽光と月光を混ぜるため、変わるのは夜だけです。値を上げると明るく見やすい夜に、下げると暗く雰囲気のある夜になります。昼光や松明などの光は変わりません。控えめな変更には 0.75～1.50 を。",
 'kr': "패스 트레이싱에서 달의 직접광 세기입니다. 게임은 시간대에 따라 햇빛과 달빛을 섞으므로 밤만 바뀝니다. 값이 높으면 밝고 잘 보이는 밤, 낮으면 어둡고 분위기 있는 밤이 됩니다. 낮, 횃불 등 다른 빛은 그대로입니다. 은은하게 바꾸려면 0.75~1.50.",
 'cn': "路径追踪中月亮直射光的强度。游戏按时间段混合日光和月光，因此只改变夜晚。数值越高夜晚越亮、越清晰；越低越暗、越有氛围。白天、火把和其他光源不变。想要细微变化可试 0.75–1.50。",
 'zh': "路徑追蹤中月亮直射光的強度。遊戲依時段混合日光與月光，因此只改變夜晚。數值越高夜晚越亮、越清晰；越低越暗、越有氛圍。白天、火把與其他光源不變。想要細微變化可試 0.75–1.50。",
 'ar': "شدة ضوء القمر المباشر في تتبع المسار. تمزج اللعبة ضوء الشمس والقمر حسب وقت اليوم، لذا تتغير الليالي فقط. القيم الأعلى تعطي ليالي أكثر إضاءة والأدنى ليالي أكثر ظلمة. ضوء النهار والمشاعل والأضواء الأخرى لا تتغير. جرّب 0.75–1.50 لتغيير خفيف.",
}

# rev4: the trace-mode description now ends with the next-start note; its last sentence repeated the appended
# "The FPS effect varies by scene" and is dropped to stay within the tooltip area.
def _drop_last(text, lang):
    parts = [p for p in re.split(r'(?<=[.。])\s*', text.strip()) if p]
    return ('' if lang in ('jp', 'cn', 'zh') else ' ').join(parts[:-1])
CORE['URTX_RTASFastTrace'] = {l: _drop_last(t, l) for l, t in CORE['URTX_RTASFastTrace'].items()}

# option labels (new keys only)
LABELS = {
 'urt531_fasttrace': {'en': 'Fast Trace', 'de': 'Schnelles Tracing', 'fr': 'Tracé rapide', 'es': 'Trazado rápido', 'esmx': 'Trazado rápido',
   'it': 'Tracciamento rapido', 'pl': 'Szybkie śledzenie', 'ru': 'Быстрая трассировка', 'ua': 'Швидке трасування', 'cz': 'Rychlé trasování',
   'hu': 'Gyors követés', 'tr': 'Hızlı izleme', 'br': 'Traçado rápido', 'jp': '高速トレース', 'kr': '빠른 추적', 'cn': '快速追踪', 'zh': '快速追蹤',
   'ar': 'تتبع سريع'},
 'urt531_fastbuild': {'en': 'Fast Build', 'de': 'Schneller Aufbau', 'fr': 'Construction rapide', 'es': 'Construcción rápida', 'esmx': 'Construcción rápida',
   'it': 'Costruzione rapida', 'pl': 'Szybka budowa', 'ru': 'Быстрая сборка', 'ua': 'Швидка збірка', 'cz': 'Rychlé sestavení',
   'hu': 'Gyors építés', 'tr': 'Hızlı oluşturma', 'br': 'Construção rápida', 'jp': '高速ビルド', 'kr': '빠른 빌드', 'cn': '快速构建', 'zh': '快速建構',
   'ar': 'بناء سريع'},
}

# R7 language-neutral value labels (Latin digits, as every URT value label)
for _k, _v in {'urt537_n32': '32', 'urt537_n16': '16', 'urt537_n2': '2', 'urt537_p1': '1%', 'urt537_p2': '2%', 'urt537_p5': '5%',
               'urt11_p50': '50%', 'urt11_p25': '25%', 'urt12_p75': '75%'}.items():   # R11/R12: PT coverage rows
    LABELS[_k] = {_l: _v for _l in ['ar', 'br', 'cn', 'cz', 'de', 'en', 'es', 'esmx', 'fr', 'hu', 'it', 'jp', 'kr', 'pl', 'ru', 'tr', 'ua', 'zh']}
