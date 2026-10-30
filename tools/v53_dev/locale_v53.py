# -*- coding: utf-8 -*-
"""V5.3 private development strings (18 languages). Conventions of V5.2.0 kept: row names in English in
every language (like every URTX row), translated tooltips and translated named headings padded with a
leading space and a trailing no-break space, "Path Tracing", "Normal RT" and "FPS" untranslated.
Every tooltip says: what it changes; FPS / frame-time / memory / visual effect; trade-off; renderer scope;
when it applies. No FPS gain is claimed without evidence."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'v52_selected'))
import spec as S

LANGS = ['ar', 'br', 'cn', 'cz', 'de', 'en', 'es', 'esmx', 'fr', 'hu', 'it', 'jp', 'kr', 'pl', 'ru', 'tr', 'ua', 'zh']
# language-neutral option values (Latin digits, as every existing URT value label)
VALUES = {'urt53_x0p5': '0.5x', 'urt53_x2': '2x', 'urt53_x4': '4x', 'urt53_m5': '5 m', 'urt53_m30': '30 m', 'urt53_m60': '60 m',
          'urt53_m90': '90 m', 'urt53_n200': '200', 'urt53_n800': '800', 'urt53_n1600': '1600', 'urt53_n600': '600',
          'urt53_n1200': '1200', 'urt53_n2400': '2400', 'urt53_n3600': '3600', 'urt53_p10': '10%', 'urt53_p20': '20%',
          'urt53_p50': '50%', 'urt53_n128': '128', 'urt53_n192': '192', 'urt53_n6': '6', 'urt53_n7': '7'}

H = {  # named headings
 'en': ('Memory & Streaming', 'RT Memory Management'),
 'ar': ('الذاكرة والبث', 'إدارة ذاكرة RT'),
 'br': ('Memória e streaming', 'Gestão de memória RT'),
 'cn': ('内存与流式加载', 'RT 内存管理'),
 'cz': ('Paměť a streamování', 'Správa paměti RT'),
 'de': ('Speicher & Streaming', 'RT-Speicherverwaltung'),
 'es': ('Memoria y streaming', 'Gestión de memoria RT'),
 'esmx': ('Memoria y streaming', 'Gestión de memoria RT'),
 'fr': ('Mémoire et streaming', 'Gestion mémoire RT'),
 'hu': ('Memória és streamelés', 'RT-memóriakezelés'),
 'it': ('Memoria e streaming', 'Gestione memoria RT'),
 'jp': ('メモリとストリーミング', 'RT メモリ管理'),
 'kr': ('메모리 및 스트리밍', 'RT 메모리 관리'),
 'pl': ('Pamięć i streaming', 'Zarządzanie pamięcią RT'),
 'ru': ('Память и стриминг', 'Управление памятью RT'),
 'tr': ('Bellek ve akış', 'RT belleği'),
 'ua': ("Пам'ять і стримінг", "Керування пам'яттю RT"),
 'zh': ('記憶體與串流', 'RT 記憶體管理'),
}

T = {}  # tooltips: row id -> {lang: text}
T['URTX_TexUnloadSpeed'] = {
 'en': 'How fast textures that are no longer needed are unloaded (native 1x). Faster unloading lowers video memory use, which can steady frame times near the VRAM limit; too fast may make textures reload (brief blur or pop-in) when you look back. No direct FPS gain. All renderers. Applies immediately.',
 'de': 'Wie schnell nicht mehr benötigte Texturen entladen werden (nativ 1x). Schnelleres Entladen senkt den Videospeicherbedarf und kann Frametimes nahe am VRAM-Limit stabilisieren; zu schnell kann Texturen beim Zurückblicken neu laden (kurz unscharf oder Pop-in). Kein direkter FPS-Gewinn. Alle Renderer. Wirkt sofort.',
 'fr': "Vitesse de déchargement des textures devenues inutiles (natif 1x). Un déchargement plus rapide réduit l'utilisation de la mémoire vidéo et peut stabiliser les temps d'image près de la limite de VRAM ; trop rapide, les textures peuvent se recharger (flou bref ou pop-in) quand vous vous retournez. Pas de gain de FPS direct. Tous les rendus. S'applique immédiatement.",
 'es': 'Rapidez con la que se descargan las texturas que ya no se necesitan (nativo 1x). Descargar más rápido reduce el uso de memoria de vídeo y puede estabilizar los tiempos de fotograma cerca del límite de VRAM; demasiado rápido puede hacer que las texturas se recarguen (breve desenfoque o pop-in) al volver la vista. Sin mejora directa de FPS. Todos los renderizadores. Se aplica de inmediato.',
 'it': "Velocità con cui vengono scaricate le texture non più necessarie (nativo 1x). Uno scaricamento più rapido riduce l'uso della memoria video e può stabilizzare i tempi dei fotogrammi vicino al limite di VRAM; troppo rapido può far ricaricare le texture (breve sfocatura o pop-in) quando ti volti. Nessun aumento diretto degli FPS. Tutti i renderer. Si applica subito.",
 'pl': 'Jak szybko zwalniane są niepotrzebne już tekstury (natywnie 1x). Szybsze zwalnianie zmniejsza użycie pamięci wideo i może ustabilizować czasy klatek blisko limitu VRAM; zbyt szybkie może powodować ponowne wczytywanie tekstur (krótkie rozmycie lub doczytywanie) po odwróceniu się. Brak bezpośredniego wzrostu FPS. Wszystkie renderery. Działa od razu.',
 'ru': 'Как быстро выгружаются больше не нужные текстуры (по умолчанию 1x). Более быстрая выгрузка снижает расход видеопамяти и может стабилизировать время кадра у предела VRAM; слишком быстрая может вызвать повторную загрузку текстур (кратковременное размытие или появление на глазах), когда вы оборачиваетесь. Прямого прироста FPS нет. Все рендереры. Применяется сразу.',
 'ua': 'Як швидко вивантажуються більше не потрібні текстури (типово 1x). Швидше вивантаження зменшує використання відеопам\'яті й може стабілізувати час кадру біля межі VRAM; занадто швидке може спричинити повторне завантаження текстур (коротке розмиття або появу на очах), коли ви обертаєтеся. Прямого приросту FPS немає. Усі рендерери. Застосовується одразу.',
 'cz': 'Jak rychle se uvolňují textury, které už nejsou potřeba (nativně 1x). Rychlejší uvolňování snižuje využití videopaměti a může ustálit časy snímků blízko limitu VRAM; příliš rychlé může způsobit opětovné načítání textur (krátké rozmazání nebo doskakování), když se otočíte. Žádný přímý zisk FPS. Všechny renderery. Projeví se okamžitě.',
 'hu': 'Milyen gyorsan töltődnek ki a már nem szükséges textúrák (natív 1x). A gyorsabb kitöltés csökkenti a videomemória-használatot, és stabilizálhatja a képkockaidőt a VRAM-korlát közelében; túl gyors esetén a textúrák újratöltődhetnek (rövid elmosódás vagy beugrás), ha visszanézel. Nincs közvetlen FPS-nyereség. Minden renderelő. Azonnal érvényes.',
 'tr': 'Artık gerekmeyen dokuların ne kadar hızlı boşaltıldığı (yerel 1x). Daha hızlı boşaltma video belleği kullanımını azaltır ve VRAM sınırına yakınken kare sürelerini dengeleyebilir; çok hızlıysa geri baktığınızda dokular yeniden yüklenebilir (kısa bulanıklık veya aniden belirme). Doğrudan FPS kazancı yok. Tüm işleyiciler. Hemen uygulanır.',
 'br': 'Velocidade com que as texturas que não são mais necessárias são descarregadas (nativo 1x). Descarregar mais rápido reduz o uso de memória de vídeo e pode estabilizar os tempos de quadro perto do limite de VRAM; rápido demais pode fazer as texturas recarregarem (breve desfoque ou pop-in) quando você olha para trás. Sem ganho direto de FPS. Todos os renderizadores. Aplica-se imediatamente.',
 'esmx': 'Rapidez con la que se descargan las texturas que ya no se necesitan (nativo 1x). Descargar más rápido reduce el uso de memoria de video y puede estabilizar los tiempos de cuadro cerca del límite de VRAM; demasiado rápido puede hacer que las texturas se vuelvan a cargar (breve desenfoque o pop-in) al voltear. Sin mejora directa de FPS. Todos los renderizadores. Se aplica de inmediato.',
 'jp': '不要になったテクスチャを解放する速さです（ネイティブ 1x）。速くするとビデオメモリ使用量が減り、VRAM 上限付近でフレームタイムが安定する場合があります。速すぎると振り返ったときにテクスチャが再読み込みされることがあります（一瞬のぼやけやポップイン）。FPS の直接的な向上はありません。すべてのレンダラー。すぐに反映されます。',
 'kr': '더 이상 필요 없는 텍스처를 해제하는 속도입니다(기본 1x). 빠르게 해제하면 비디오 메모리 사용량이 줄어 VRAM 한계 근처에서 프레임 타임이 안정될 수 있습니다. 너무 빠르면 뒤돌아볼 때 텍스처가 다시 로드될 수 있습니다(잠깐의 흐림 또는 팝인). 직접적인 FPS 향상은 없습니다. 모든 렌더러. 즉시 적용됩니다.',
 'cn': '不再需要的纹理被卸载的速度（原生 1x）。更快卸载可降低显存占用，在接近显存上限时有助于稳定帧时间；过快可能在回头时重新加载纹理（短暂模糊或突然出现）。不直接提升 FPS。所有渲染器。立即生效。',
 'zh': '不再需要的材質貼圖被卸載的速度（原生 1x）。更快卸載可降低顯示記憶體用量，在接近 VRAM 上限時有助於穩定影格時間；過快可能在回頭時重新載入材質（短暫模糊或突然出現）。不直接提升 FPS。所有渲染器。立即生效。',
 'ar': 'سرعة إلغاء تحميل الخامات التي لم تعد مطلوبة (الأصلي 1x). الإلغاء الأسرع يقلل استخدام ذاكرة الفيديو وقد يثبت أزمنة الإطارات قرب حد VRAM؛ السرعة الزائدة قد تعيد تحميل الخامات (ضبابية قصيرة أو ظهور مفاجئ) عند الالتفات. لا تحسن مباشر في FPS. كل أنماط العرض. يُطبق فورًا.',
}
T['URTX_MeshStreamLead'] = {
 'en': "How far before an object's view distance its mesh starts loading (native 10 m). Lower saves memory and streaming work but may cause pop-in, mostly on horseback; higher reduces pop-in but uses more memory. No direct FPS gain. All renderers. Applies to newly streamed areas.",
 'de': 'Wie weit vor der Sichtweite eines Objekts sein Mesh zu laden beginnt (nativ 10 m). Niedriger spart Speicher und Streaming-Arbeit, kann aber Pop-in verursachen, vor allem zu Pferd; höher verringert Pop-in, braucht aber mehr Speicher. Kein direkter FPS-Gewinn. Alle Renderer. Wirkt für neu gestreamte Gebiete.',
 'fr': "Distance avant la portée de vue d'un objet à laquelle son maillage commence à charger (natif 10 m). Plus bas économise mémoire et streaming mais peut causer du pop-in, surtout à cheval ; plus haut réduit le pop-in mais utilise plus de mémoire. Pas de gain de FPS direct. Tous les rendus. S'applique aux zones nouvellement chargées.",
 'es': 'A qué distancia antes del alcance de visión de un objeto empieza a cargarse su malla (nativo 10 m). Menos ahorra memoria y trabajo de streaming pero puede causar pop-in, sobre todo a caballo; más reduce el pop-in pero usa más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica a las zonas cargadas de nuevo.',
 'esmx': 'A qué distancia antes del alcance de visión de un objeto empieza a cargarse su malla (nativo 10 m). Menos ahorra memoria y trabajo de streaming pero puede causar pop-in, sobre todo a caballo; más reduce el pop-in pero usa más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica a las zonas cargadas de nuevo.',
 'it': "Quanto prima della distanza visiva di un oggetto inizia il caricamento della sua mesh (nativo 10 m). Più basso risparmia memoria e streaming ma può causare pop-in, soprattutto a cavallo; più alto riduce il pop-in ma usa più memoria. Nessun aumento diretto degli FPS. Tutti i renderer. Si applica alle aree caricate di nuovo.",
 'pl': 'Jak daleko przed zasięgiem widzenia obiektu zaczyna się wczytywanie jego siatki (natywnie 10 m). Mniej oszczędza pamięć i strumieniowanie, ale może powodować doczytywanie, zwłaszcza konno; więcej zmniejsza doczytywanie, ale zużywa więcej pamięci. Brak bezpośredniego wzrostu FPS. Wszystkie renderery. Działa w nowo wczytanych obszarach.',
 'ru': 'За сколько метров до дальности видимости объекта начинает загружаться его меш (по умолчанию 10 м). Меньше экономит память и потоковую загрузку, но может вызвать появление объектов на глазах, особенно верхом; больше уменьшает это, но расходует больше памяти. Прямого прироста FPS нет. Все рендереры. Применяется к заново загруженным областям.',
 'ua': 'За скільки метрів до дальності видимості об\'єкта починає завантажуватися його меш (типово 10 м). Менше заощаджує пам\'ять і потокове завантаження, але може спричинити появу об\'єктів на очах, особливо верхи; більше зменшує це, але витрачає більше пам\'яті. Прямого приросту FPS немає. Усі рендерери. Застосовується до щойно завантажених областей.',
 'cz': 'Jak daleko před dohledností objektu se začne načítat jeho síť (nativně 10 m). Méně šetří paměť a streamování, ale může způsobit doskakování, hlavně na koni; více doskakování omezí, ale spotřebuje víc paměti. Žádný přímý zisk FPS. Všechny renderery. Projeví se v nově načtených oblastech.',
 'hu': 'Egy objektum látótávolsága előtt mennyivel kezd betöltődni a modellje (natív 10 m). Kisebb érték memóriát és streamelést spórol, de beugrást okozhat, főleg lóháton; nagyobb csökkenti a beugrást, de több memóriát használ. Nincs közvetlen FPS-nyereség. Minden renderelő. Az újonnan betöltött területeken érvényes.',
 'tr': 'Bir nesnenin görüş mesafesinden ne kadar önce modelinin yüklenmeye başladığı (yerel 10 m). Düşük değer bellek ve akış işinden tasarruf eder ama özellikle at üstünde aniden belirmeye yol açabilir; yüksek değer bunu azaltır ama daha fazla bellek kullanır. Doğrudan FPS kazancı yok. Tüm işleyiciler. Yeni yüklenen alanlara uygulanır.',
 'br': 'Quanto antes da distância de visão de um objeto a malha dele começa a carregar (nativo 10 m). Menor economiza memória e streaming, mas pode causar pop-in, principalmente a cavalo; maior reduz o pop-in, mas usa mais memória. Sem ganho direto de FPS. Todos os renderizadores. Aplica-se às áreas recém-carregadas.',
 'jp': 'オブジェクトの表示距離のどれだけ手前でメッシュの読み込みを始めるかです（ネイティブ 10 m）。下げるとメモリとストリーミング負荷を節約できますが、特に乗馬中にポップインが起きる場合があります。上げるとポップインが減りますがメモリを多く使います。FPS の直接的な向上はありません。すべてのレンダラー。新しく読み込まれたエリアに反映されます。',
 'kr': '오브젝트의 표시 거리보다 얼마나 앞에서 메시 로딩을 시작하는지입니다(기본 10 m). 낮추면 메모리와 스트리밍 작업이 줄지만 특히 말을 탈 때 팝인이 생길 수 있고, 높이면 팝인이 줄지만 메모리를 더 사용합니다. 직접적인 FPS 향상은 없습니다. 모든 렌더러. 새로 불러온 지역에 적용됩니다.',
 'cn': '物体在其可视距离之前多远开始加载网格（原生 10 m）。降低可节省内存和流式加载工作，但可能出现物体突然出现，骑马时尤甚；提高可减少此现象，但占用更多内存。不直接提升 FPS。所有渲染器。对新加载的区域生效。',
 'zh': '物件在其可視距離之前多遠開始載入網格（原生 10 m）。降低可節省記憶體與串流工作，但可能出現物件突然出現，騎馬時尤甚；提高可減少此現象，但占用更多記憶體。不直接提升 FPS。所有渲染器。對新載入的區域生效。',
 'ar': 'كم قبل مسافة رؤية الكائن يبدأ تحميل مجسمه (الأصلي 10 m). القيمة الأقل توفر الذاكرة وعمل البث لكنها قد تسبب ظهورًا مفاجئًا، خاصة على الحصان؛ والأعلى تقلل ذلك لكنها تستخدم ذاكرة أكثر. لا تحسن مباشر في FPS. كل أنماط العرض. يُطبق على المناطق المحملة حديثًا.',
}
T['URTX_GenericStreamLead'] = {
 'en': 'The same lead distance for everything else in a world sector (native 45 m). Lower saves memory and streaming work but things may appear later; higher reduces pop-in but uses more memory. No direct FPS gain. All renderers. Applies to newly streamed areas.',
 'de': 'Derselbe Vorlaufabstand für alles andere in einem Weltsektor (nativ 45 m). Niedriger spart Speicher und Streaming-Arbeit, Dinge können aber später erscheinen; höher verringert Pop-in, braucht aber mehr Speicher. Kein direkter FPS-Gewinn. Alle Renderer. Wirkt für neu gestreamte Gebiete.',
 'fr': "La même distance d'anticipation pour tout le reste d'un secteur du monde (natif 45 m). Plus bas économise mémoire et streaming mais les éléments peuvent apparaître plus tard ; plus haut réduit le pop-in mais utilise plus de mémoire. Pas de gain de FPS direct. Tous les rendus. S'applique aux zones nouvellement chargées.",
 'es': 'La misma distancia de anticipación para todo lo demás de un sector del mundo (nativo 45 m). Menos ahorra memoria y streaming pero los elementos pueden aparecer más tarde; más reduce el pop-in pero usa más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica a las zonas cargadas de nuevo.',
 'esmx': 'La misma distancia de anticipación para todo lo demás de un sector del mundo (nativo 45 m). Menos ahorra memoria y streaming pero los elementos pueden aparecer más tarde; más reduce el pop-in pero usa más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica a las zonas cargadas de nuevo.',
 'it': 'La stessa distanza di anticipo per tutto il resto di un settore del mondo (nativo 45 m). Più bassa risparmia memoria e streaming ma gli elementi possono comparire più tardi; più alta riduce il pop-in ma usa più memoria. Nessun aumento diretto degli FPS. Tutti i renderer. Si applica alle aree caricate di nuovo.',
 'pl': 'Ten sam dystans wyprzedzenia dla wszystkiego innego w sektorze świata (natywnie 45 m). Mniej oszczędza pamięć i strumieniowanie, ale obiekty mogą pojawiać się później; więcej zmniejsza doczytywanie, ale zużywa więcej pamięci. Brak bezpośredniego wzrostu FPS. Wszystkie renderery. Działa w nowo wczytanych obszarach.',
 'ru': 'То же упреждение для всего остального в секторе мира (по умолчанию 45 м). Меньше экономит память и потоковую загрузку, но объекты могут появляться позже; больше уменьшает появление на глазах, но расходует больше памяти. Прямого прироста FPS нет. Все рендереры. Применяется к заново загруженным областям.',
 'ua': 'Те саме випередження для всього іншого в секторі світу (типово 45 м). Менше заощаджує пам\'ять і потокове завантаження, але об\'єкти можуть з\'являтися пізніше; більше зменшує появу на очах, але витрачає більше пам\'яті. Прямого приросту FPS немає. Усі рендерери. Застосовується до щойно завантажених областей.',
 'cz': 'Stejný předstih pro vše ostatní v sektoru světa (nativně 45 m). Méně šetří paměť a streamování, ale věci se mohou objevit později; více omezí doskakování, ale spotřebuje víc paměti. Žádný přímý zisk FPS. Všechny renderery. Projeví se v nově načtených oblastech.',
 'hu': 'Ugyanez az előtöltési távolság a világszektor minden más elemére (natív 45 m). Kisebb érték memóriát és streamelést spórol, de az elemek később jelenhetnek meg; nagyobb csökkenti a beugrást, de több memóriát használ. Nincs közvetlen FPS-nyereség. Minden renderelő. Az újonnan betöltött területeken érvényes.',
 'tr': 'Bir dünya bölgesindeki diğer her şey için aynı ön yükleme mesafesi (yerel 45 m). Düşük değer bellek ve akış işinden tasarruf eder ama nesneler daha geç belirebilir; yüksek değer aniden belirmeyi azaltır ama daha fazla bellek kullanır. Doğrudan FPS kazancı yok. Tüm işleyiciler. Yeni yüklenen alanlara uygulanır.',
 'br': 'A mesma distância de antecipação para todo o resto de um setor do mundo (nativo 45 m). Menor economiza memória e streaming, mas as coisas podem aparecer mais tarde; maior reduz o pop-in, mas usa mais memória. Sem ganho direto de FPS. Todos os renderizadores. Aplica-se às áreas recém-carregadas.',
 'jp': 'ワールドセクター内のその他すべてに対する同じ先読み距離です（ネイティブ 45 m）。下げるとメモリとストリーミング負荷を節約できますが、物の表示が遅れる場合があります。上げるとポップインが減りますがメモリを多く使います。FPS の直接的な向上はありません。すべてのレンダラー。新しく読み込まれたエリアに反映されます。',
 'kr': '월드 섹터의 나머지 모든 것에 대한 같은 선행 거리입니다(기본 45 m). 낮추면 메모리와 스트리밍 작업이 줄지만 사물이 늦게 나타날 수 있고, 높이면 팝인이 줄지만 메모리를 더 사용합니다. 직접적인 FPS 향상은 없습니다. 모든 렌더러. 새로 불러온 지역에 적용됩니다.',
 'cn': '世界区块中其他所有内容的相同提前距离（原生 45 m）。降低可节省内存和流式加载工作，但内容可能更晚出现；提高可减少突然出现，但占用更多内存。不直接提升 FPS。所有渲染器。对新加载的区域生效。',
 'zh': '世界區塊中其他所有內容的相同提前距離（原生 45 m）。降低可節省記憶體與串流工作，但內容可能更晚出現；提高可減少突然出現，但占用更多記憶體。不直接提升 FPS。所有渲染器。對新載入的區域生效。',
 'ar': 'نفس مسافة التقدم لكل ما عدا ذلك في قطاع العالم (الأصلي 45 m). القيمة الأقل توفر الذاكرة وعمل البث لكن الأشياء قد تظهر متأخرة؛ والأعلى تقلل الظهور المفاجئ لكنها تستخدم ذاكرة أكثر. لا تحسن مباشر في FPS. كل أنماط العرض. يُطبق على المناطق المحملة حديثًا.',
}
T['URTX_GCDiscardThreshold'] = {
 'en': 'How many discarded game objects trigger an automatic memory cleanup (garbage collection). Lower frees memory sooner but cleans up more often (possible brief hitches); higher cleans up less often but holds more memory. No direct FPS gain. All renderers. Applies immediately.',
 'de': 'Wie viele verworfene Spielobjekte eine automatische Speicherbereinigung auslösen (Garbage Collection). Niedriger gibt Speicher früher frei, bereinigt aber öfter (mögliche kurze Ruckler); höher bereinigt seltener, hält aber mehr Speicher. Kein direkter FPS-Gewinn. Alle Renderer. Wirkt sofort.',
 'fr': "Nombre d'objets de jeu abandonnés qui déclenche un nettoyage mémoire automatique (ramasse-miettes). Plus bas libère la mémoire plus tôt mais nettoie plus souvent (brèves saccades possibles) ; plus haut nettoie moins souvent mais garde plus de mémoire. Pas de gain de FPS direct. Tous les rendus. S'applique immédiatement.",
 'es': 'Cuántos objetos del juego descartados activan una limpieza automática de memoria (recolección de basura). Menos libera memoria antes pero limpia más a menudo (posibles tirones breves); más limpia menos a menudo pero retiene más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica de inmediato.',
 'esmx': 'Cuántos objetos del juego descartados activan una limpieza automática de memoria (recolección de basura). Menos libera memoria antes pero limpia más seguido (posibles tirones breves); más limpia menos seguido pero retiene más memoria. Sin mejora directa de FPS. Todos los renderizadores. Se aplica de inmediato.',
 'it': 'Quanti oggetti di gioco scartati avviano una pulizia automatica della memoria (garbage collection). Più basso libera memoria prima ma pulisce più spesso (possibili brevi scatti); più alto pulisce meno spesso ma trattiene più memoria. Nessun aumento diretto degli FPS. Tutti i renderer. Si applica subito.',
 'pl': 'Ile porzuconych obiektów gry uruchamia automatyczne czyszczenie pamięci (garbage collection). Mniej zwalnia pamięć wcześniej, ale czyści częściej (możliwe krótkie przycięcia); więcej czyści rzadziej, ale trzyma więcej pamięci. Brak bezpośredniego wzrostu FPS. Wszystkie renderery. Działa od razu.',
 'ru': 'Сколько отброшенных игровых объектов запускает автоматическую очистку памяти (сборку мусора). Меньше освобождает память раньше, но очищает чаще (возможны короткие подтормаживания); больше очищает реже, но удерживает больше памяти. Прямого прироста FPS нет. Все рендереры. Применяется сразу.',
 'ua': 'Скільки відкинутих ігрових об\'єктів запускає автоматичне очищення пам\'яті (збирання сміття). Менше звільняє пам\'ять раніше, але очищує частіше (можливі короткі підгальмовування); більше очищує рідше, але утримує більше пам\'яті. Прямого приросту FPS немає. Усі рендерери. Застосовується одразу.',
 'cz': 'Kolik vyřazených herních objektů spustí automatické čištění paměti (garbage collection). Méně uvolní paměť dříve, ale čistí častěji (možná krátká zaškobrtnutí); více čistí méně často, ale drží více paměti. Žádný přímý zisk FPS. Všechny renderery. Projeví se okamžitě.',
 'hu': 'Hány eldobott játékobjektum indít automatikus memóriatisztítást (szemétgyűjtés). Kisebb érték hamarabb szabadít fel memóriát, de gyakrabban tisztít (rövid akadások lehetnek); nagyobb ritkábban tisztít, de több memóriát tart. Nincs közvetlen FPS-nyereség. Minden renderelő. Azonnal érvényes.',
 'tr': 'Kaç atılmış oyun nesnesinin otomatik bellek temizliğini tetiklediği (çöp toplama). Düşük değer belleği daha erken boşaltır ama daha sık temizler (kısa takılmalar olabilir); yüksek değer daha seyrek temizler ama daha fazla bellek tutar. Doğrudan FPS kazancı yok. Tüm işleyiciler. Hemen uygulanır.',
 'br': 'Quantos objetos de jogo descartados acionam uma limpeza automática de memória (coleta de lixo). Menor libera memória antes, mas limpa com mais frequência (possíveis travadas breves); maior limpa com menos frequência, mas retém mais memória. Sem ganho direto de FPS. Todos os renderizadores. Aplica-se imediatamente.',
 'jp': '破棄されたゲームオブジェクトがいくつたまると自動メモリクリーンアップ（ガベージコレクション）を行うかです。下げると早くメモリを解放しますが頻繁に実行されます（短いヒッチの可能性）。上げると実行回数は減りますがメモリを多く保持します。FPS の直接的な向上はありません。すべてのレンダラー。すぐに反映されます。',
 'kr': '버려진 게임 오브젝트가 몇 개 쌓이면 자동 메모리 정리(가비지 컬렉션)를 실행하는지입니다. 낮추면 메모리를 일찍 해제하지만 더 자주 정리합니다(짧은 끊김 가능). 높이면 덜 자주 정리하지만 메모리를 더 많이 보유합니다. 직접적인 FPS 향상은 없습니다. 모든 렌더러. 즉시 적용됩니다.',
 'cn': '累积多少个被丢弃的游戏对象时触发自动内存清理（垃圾回收）。降低可更早释放内存，但清理更频繁（可能短暂卡顿）；提高则清理较少，但占用更多内存。不直接提升 FPS。所有渲染器。立即生效。',
 'zh': '累積多少個被丟棄的遊戲物件時觸發自動記憶體清理（垃圾回收）。降低可更早釋放記憶體，但清理更頻繁（可能短暫卡頓）；提高則清理較少，但占用更多記憶體。不直接提升 FPS。所有渲染器。立即生效。',
 'ar': 'عدد كائنات اللعبة المستبعدة الذي يطلق تنظيفًا تلقائيًا للذاكرة (جمع البيانات المهملة). القيمة الأقل تحرر الذاكرة أسرع لكنها تنظف أكثر (تقطعات قصيرة محتملة)؛ والأعلى تنظف أقل لكنها تحتفظ بذاكرة أكثر. لا تحسن مباشر في FPS. كل أنماط العرض. يُطبق فورًا.',
}
T['URTX_GCIdleFrames'] = {
 'en': 'Minimum number of frames between automatic memory cleanups (garbage collection). Lower frees memory more often but can add periodic stutter; higher means fewer interruptions but more memory held. Native / Auto keeps the value from the game\'s own configuration. All renderers. Applies immediately.',
 'de': 'Minimale Anzahl Frames zwischen automatischen Speicherbereinigungen (Garbage Collection). Niedriger gibt Speicher öfter frei, kann aber periodische Ruckler verursachen; höher bedeutet weniger Unterbrechungen, aber mehr gehaltenen Speicher. Nativ / Auto behält den Wert aus der Spielkonfiguration. Alle Renderer. Wirkt sofort.',
 'fr': "Nombre minimal d'images entre deux nettoyages mémoire automatiques (ramasse-miettes). Plus bas libère la mémoire plus souvent mais peut ajouter des saccades périodiques ; plus haut réduit les interruptions mais garde plus de mémoire. Natif / Auto garde la valeur de la configuration du jeu. Tous les rendus. S'applique immédiatement.",
 'es': 'Número mínimo de fotogramas entre limpiezas automáticas de memoria (recolección de basura). Menos libera memoria más a menudo pero puede añadir tirones periódicos; más significa menos interrupciones pero más memoria retenida. Nativo / Auto mantiene el valor de la configuración del juego. Todos los renderizadores. Se aplica de inmediato.',
 'esmx': 'Número mínimo de cuadros entre limpiezas automáticas de memoria (recolección de basura). Menos libera memoria más seguido pero puede añadir tirones periódicos; más significa menos interrupciones pero más memoria retenida. Nativo / Auto mantiene el valor de la configuración del juego. Todos los renderizadores. Se aplica de inmediato.',
 'it': 'Numero minimo di fotogrammi tra due pulizie automatiche della memoria (garbage collection). Più basso libera memoria più spesso ma può aggiungere scatti periodici; più alto significa meno interruzioni ma più memoria trattenuta. Nativo / Auto mantiene il valore della configurazione del gioco. Tutti i renderer. Si applica subito.',
 'pl': 'Minimalna liczba klatek między automatycznymi czyszczeniami pamięci (garbage collection). Mniej zwalnia pamięć częściej, ale może dodać okresowe przycięcia; więcej oznacza mniej przerw, ale więcej zajętej pamięci. Natywne / Auto zachowuje wartość z konfiguracji gry. Wszystkie renderery. Działa od razu.',
 'ru': 'Минимальное число кадров между автоматическими очистками памяти (сборкой мусора). Меньше освобождает память чаще, но может добавить периодические подтормаживания; больше — меньше прерываний, но больше удерживаемой памяти. По умолчанию / Авто сохраняет значение из конфигурации игры. Все рендереры. Применяется сразу.',
 'ua': 'Мінімальна кількість кадрів між автоматичними очищеннями пам\'яті (збиранням сміття). Менше звільняє пам\'ять частіше, але може додати періодичні підгальмовування; більше — менше переривань, але більше утримуваної пам\'яті. Типово / Авто зберігає значення з конфігурації гри. Усі рендерери. Застосовується одразу.',
 'cz': 'Minimální počet snímků mezi automatickými čištěními paměti (garbage collection). Méně uvolňuje paměť častěji, ale může přidat pravidelná zaškobrtnutí; více znamená méně přerušení, ale více držené paměti. Nativní / Auto ponechá hodnotu z konfigurace hry. Všechny renderery. Projeví se okamžitě.',
 'hu': 'Az automatikus memóriatisztítások (szemétgyűjtés) közötti minimális képkockaszám. Kisebb érték gyakrabban szabadít fel memóriát, de időszakos akadást okozhat; nagyobb kevesebb megszakítást, de több lefoglalt memóriát jelent. A Natív / Auto a játék saját beállításának értékét tartja meg. Minden renderelő. Azonnal érvényes.',
 'tr': 'Otomatik bellek temizlikleri (çöp toplama) arasındaki en az kare sayısı. Düşük değer belleği daha sık boşaltır ama periyodik takılma ekleyebilir; yüksek değer daha az kesinti ama daha fazla tutulan bellek demektir. Yerel / Otomatik oyunun kendi yapılandırmasındaki değeri korur. Tüm işleyiciler. Hemen uygulanır.',
 'br': 'Número mínimo de quadros entre limpezas automáticas de memória (coleta de lixo). Menor libera memória com mais frequência, mas pode causar travadas periódicas; maior significa menos interrupções, mas mais memória retida. Nativo / Auto mantém o valor da configuração do jogo. Todos os renderizadores. Aplica-se imediatamente.',
 'jp': '自動メモリクリーンアップ（ガベージコレクション）の間の最小フレーム数です。下げるとメモリを頻繁に解放しますが周期的なカクつきが出る場合があります。上げると中断は減りますが保持するメモリが増えます。ネイティブ / 自動はゲーム自身の設定値を維持します。すべてのレンダラー。すぐに反映されます。',
 'kr': '자동 메모리 정리(가비지 컬렉션) 사이의 최소 프레임 수입니다. 낮추면 메모리를 더 자주 해제하지만 주기적인 끊김이 생길 수 있고, 높이면 중단은 줄지만 보유 메모리가 늘어납니다. 기본 / 자동은 게임 자체 설정값을 유지합니다. 모든 렌더러. 즉시 적용됩니다.',
 'cn': '两次自动内存清理（垃圾回收）之间的最少帧数。降低会更频繁地释放内存，但可能带来周期性卡顿；提高则中断更少，但占用更多内存。原生 / 自动保留游戏自身配置中的数值。所有渲染器。立即生效。',
 'zh': '兩次自動記憶體清理（垃圾回收）之間的最少影格數。降低會更頻繁地釋放記憶體，但可能帶來週期性卡頓；提高則中斷更少，但占用更多記憶體。原生 / 自動保留遊戲自身設定中的數值。所有渲染器。立即生效。',
 'ar': 'أقل عدد من الإطارات بين عمليات تنظيف الذاكرة التلقائية (جمع البيانات المهملة). القيمة الأقل تحرر الذاكرة أكثر لكنها قد تضيف تقطعًا دوريًا؛ والأعلى تعني مقاطعات أقل وذاكرة محتجزة أكثر. الأصلي / تلقائي يبقي قيمة إعدادات اللعبة نفسها. كل أنماط العرض. يُطبق فورًا.',
}
T['URTX_RTASDefrag'] = {
 'en': 'Fragmentation level at which ray-tracing acceleration-structure memory is defragmented (native 30%). Lower keeps that video memory more compact over long sessions but defragments more often (extra GPU and CPU work, possible frame-time spikes); higher defragments less often. Shared by Normal RT and Path Tracing. Takes effect after restarting the game.',
 'de': 'Fragmentierungsgrad, ab dem der Speicher der Raytracing-Beschleunigungsstrukturen defragmentiert wird (nativ 30%). Niedriger hält diesen Videospeicher in langen Sitzungen kompakter, defragmentiert aber öfter (zusätzliche GPU- und CPU-Arbeit, mögliche Frametime-Spitzen); höher defragmentiert seltener. Gemeinsam für normales RT und Path Tracing. Wirkt nach einem Neustart des Spiels.',
 'fr': "Niveau de fragmentation à partir duquel la mémoire des structures d'accélération du ray tracing est défragmentée (natif 30%). Plus bas garde cette mémoire vidéo plus compacte sur de longues sessions mais défragmente plus souvent (travail GPU et CPU en plus, pics de temps d'image possibles) ; plus haut défragmente moins souvent. Partagé par le RT normal et le Path Tracing. Prend effet après redémarrage du jeu.",
 'es': 'Nivel de fragmentación a partir del cual se desfragmenta la memoria de las estructuras de aceleración del ray tracing (nativo 30%). Menos mantiene esa memoria de vídeo más compacta en sesiones largas pero desfragmenta más a menudo (trabajo extra de GPU y CPU, posibles picos de tiempo de fotograma); más desfragmenta menos a menudo. Compartido por RT normal y Path Tracing. Surte efecto tras reiniciar el juego.',
 'esmx': 'Nivel de fragmentación a partir del cual se desfragmenta la memoria de las estructuras de aceleración del ray tracing (nativo 30%). Menos mantiene esa memoria de video más compacta en sesiones largas pero desfragmenta más seguido (trabajo extra de GPU y CPU, posibles picos de tiempo de cuadro); más desfragmenta menos seguido. Compartido por RT normal y Path Tracing. Surte efecto al reiniciar el juego.',
 'it': 'Livello di frammentazione oltre il quale la memoria delle strutture di accelerazione del ray tracing viene deframmentata (nativo 30%). Più basso mantiene quella memoria video più compatta nelle sessioni lunghe ma deframmenta più spesso (lavoro GPU e CPU extra, possibili picchi del tempo fotogramma); più alto deframmenta meno spesso. Condiviso da RT normale e Path Tracing. Ha effetto dopo il riavvio del gioco.',
 'pl': 'Poziom fragmentacji, od którego defragmentowana jest pamięć struktur akceleracji ray tracingu (natywnie 30%). Mniej utrzymuje tę pamięć wideo bardziej zwartą w długich sesjach, ale defragmentuje częściej (dodatkowa praca GPU i CPU, możliwe skoki czasu klatki); więcej defragmentuje rzadziej. Wspólne dla zwykłego RT i Path Tracing. Działa po ponownym uruchomieniu gry.',
 'ru': 'Уровень фрагментации, при котором дефрагментируется память ускоряющих структур трассировки лучей (по умолчанию 30%). Меньше держит эту видеопамять компактнее в долгих сессиях, но дефрагментирует чаще (дополнительная работа GPU и CPU, возможны скачки времени кадра); больше — реже. Общая для обычного RT и Path Tracing. Действует после перезапуска игры.',
 'ua': 'Рівень фрагментації, за якого дефрагментується пам\'ять прискорювальних структур трасування променів (типово 30%). Менше тримає цю відеопам\'ять компактнішою в довгих сесіях, але дефрагментує частіше (додаткова робота GPU і CPU, можливі стрибки часу кадру); більше — рідше. Спільна для звичайного RT і Path Tracing. Діє після перезапуску гри.',
 'cz': 'Úroveň fragmentace, při které se defragmentuje paměť akceleračních struktur ray tracingu (nativně 30%). Méně udržuje tuto videopaměť v dlouhých relacích kompaktnější, ale defragmentuje častěji (práce GPU a CPU navíc, možné špičky času snímku); více defragmentuje méně často. Společné pro běžné RT i Path Tracing. Projeví se po restartu hry.',
 'hu': 'Az a töredezettségi szint, amelynél a ray tracing gyorsítóstruktúráinak memóriája töredezettségmentesítésre kerül (natív 30%). Kisebb érték hosszú játékban tömörebben tartja ezt a videomemóriát, de gyakrabban fut (több GPU- és CPU-munka, képkockaidő-kiugrások lehetnek); nagyobb ritkábban fut. Közös a normál RT és a Path Tracing számára. A játék újraindítása után érvényes.',
 'tr': 'Işın izleme hızlandırma yapısı belleğinin birleştirildiği parçalanma düzeyi (yerel %30). Düşük değer bu video belleğini uzun oturumlarda daha derli toplu tutar ama daha sık birleştirir (ek GPU ve CPU işi, olası kare süresi sıçramaları); yüksek değer daha seyrek birleştirir. Normal RT ve Path Tracing için ortaktır. Oyun yeniden başlatıldıktan sonra etkili olur.',
 'br': 'Nível de fragmentação a partir do qual a memória das estruturas de aceleração do ray tracing é desfragmentada (nativo 30%). Menor mantém essa memória de vídeo mais compacta em sessões longas, mas desfragmenta com mais frequência (trabalho extra de GPU e CPU, possíveis picos de tempo de quadro); maior desfragmenta com menos frequência. Compartilhado por RT normal e Path Tracing. Vale após reiniciar o jogo.',
 'jp': 'レイトレーシングのアクセラレーション構造メモリをデフラグする断片化の度合いです（ネイティブ 30%）。下げると長時間のプレイでこのビデオメモリをコンパクトに保てますが、デフラグが増えます（GPU と CPU の追加負荷、フレームタイムのスパイクの可能性）。上げるとデフラグは減ります。通常の RT と Path Tracing で共有。ゲームの再起動後に有効になります。',
 'kr': '레이 트레이싱 가속 구조 메모리를 조각 모음하는 단편화 수준입니다(기본 30%). 낮추면 긴 세션에서 이 비디오 메모리를 더 압축된 상태로 유지하지만 조각 모음이 잦아집니다(추가 GPU 및 CPU 작업, 프레임 타임 급등 가능). 높이면 덜 자주 실행됩니다. 일반 RT와 Path Tracing이 공유. 게임을 다시 시작한 뒤 적용됩니다.',
 'cn': '光线追踪加速结构内存在碎片达到多少时进行整理（原生 30%）。降低可在长时间游戏中让这部分显存更紧凑，但整理更频繁（额外 GPU 和 CPU 开销，可能出现帧时间尖峰）；提高则整理更少。普通 RT 与 Path Tracing 共用。重启游戏后生效。',
 'zh': '光線追蹤加速結構記憶體在碎片達到多少時進行整理（原生 30%）。降低可在長時間遊玩時讓這部分顯示記憶體更緊湊，但整理更頻繁（額外 GPU 與 CPU 負擔，可能出現影格時間尖峰）；提高則整理更少。一般 RT 與 Path Tracing 共用。重新啟動遊戲後生效。',
 'ar': 'مستوى التجزئة الذي تُلغى عنده تجزئة ذاكرة هياكل تسريع تتبع الأشعة (الأصلي 30%). القيمة الأقل تبقي ذاكرة الفيديو هذه أكثر تراصًا في الجلسات الطويلة لكنها تلغي التجزئة أكثر (عمل إضافي على GPU وCPU وقفزات محتملة في زمن الإطار)؛ والأعلى أقل تكرارًا. مشترك بين RT العادي وPath Tracing. يسري بعد إعادة تشغيل اللعبة.',
}
T['URTX_RTASBatch'] = {
 'en': 'Maximum number of ray-tracing geometry structures built in one batch (native 256; the engine itself falls back to 128). Smaller batches spread the building work over more frames, which may reduce spikes when new areas load, but newly loaded objects may join ray-traced shadows and reflections slightly later. Shared by Normal RT and Path Tracing. Takes effect after restarting the game.',
 'de': 'Maximale Anzahl Raytracing-Geometriestrukturen, die in einem Durchgang erstellt werden (nativ 256; die Engine selbst fällt auf 128 zurück). Kleinere Durchgänge verteilen die Arbeit auf mehr Frames und können Spitzen beim Laden neuer Gebiete verringern, neu geladene Objekte erscheinen aber etwas später in RT-Schatten und -Spiegelungen. Gemeinsam für normales RT und Path Tracing. Wirkt nach einem Neustart des Spiels.',
 'fr': "Nombre maximal de structures de géométrie du ray tracing construites en un lot (natif 256 ; le moteur se rabat lui-même sur 128). Des lots plus petits répartissent le travail sur plus d'images, ce qui peut réduire les pics au chargement de nouvelles zones, mais les objets nouvellement chargés peuvent rejoindre un peu plus tard les ombres et reflets en RT. Partagé par le RT normal et le Path Tracing. Prend effet après redémarrage du jeu.",
 'es': 'Número máximo de estructuras de geometría de ray tracing construidas en un lote (nativo 256; el propio motor recurre a 128). Lotes más pequeños reparten el trabajo en más fotogramas, lo que puede reducir picos al cargar zonas nuevas, pero los objetos recién cargados pueden entrar algo más tarde en sombras y reflejos RT. Compartido por RT normal y Path Tracing. Surte efecto tras reiniciar el juego.',
 'esmx': 'Número máximo de estructuras de geometría de ray tracing construidas en un lote (nativo 256; el propio motor recurre a 128). Lotes más pequeños reparten el trabajo en más cuadros, lo que puede reducir picos al cargar zonas nuevas, pero los objetos recién cargados pueden entrar un poco después en sombras y reflejos RT. Compartido por RT normal y Path Tracing. Surte efecto al reiniciar el juego.',
 'it': 'Numero massimo di strutture geometriche del ray tracing costruite in un lotto (nativo 256; il motore stesso ripiega su 128). Lotti più piccoli distribuiscono il lavoro su più fotogrammi e possono ridurre i picchi al caricamento di nuove aree, ma gli oggetti appena caricati possono entrare un po\' più tardi in ombre e riflessi RT. Condiviso da RT normale e Path Tracing. Ha effetto dopo il riavvio del gioco.',
 'pl': 'Maksymalna liczba struktur geometrii ray tracingu budowanych w jednej partii (natywnie 256; sam silnik w razie potrzeby używa 128). Mniejsze partie rozkładają pracę na więcej klatek, co może zmniejszyć skoki przy wczytywaniu nowych obszarów, ale nowo wczytane obiekty mogą nieco później trafiać do cieni i odbić RT. Wspólne dla zwykłego RT i Path Tracing. Działa po ponownym uruchomieniu gry.',
 'ru': 'Максимальное число геометрических структур трассировки лучей, строящихся за один пакет (по умолчанию 256; сам движок в запасном варианте использует 128). Меньшие пакеты распределяют работу на больше кадров и могут уменьшить скачки при загрузке новых областей, но новые объекты могут чуть позже появляться в RT-тенях и отражениях. Общая для обычного RT и Path Tracing. Действует после перезапуска игры.',
 'ua': 'Максимальна кількість геометричних структур трасування променів, що будуються за один пакет (типово 256; сам рушій у запасному варіанті використовує 128). Менші пакети розподіляють роботу на більше кадрів і можуть зменшити стрибки під час завантаження нових областей, але нові об\'єкти можуть трохи пізніше з\'являтися в RT-тінях і відображеннях. Спільна для звичайного RT і Path Tracing. Діє після перезапуску гри.',
 'cz': 'Maximální počet geometrických struktur ray tracingu sestavených v jedné dávce (nativně 256; samotný engine se vrací k 128). Menší dávky rozloží práci do více snímků a mohou omezit špičky při načítání nových oblastí, nově načtené objekty se ale mohou do RT stínů a odrazů dostat o něco později. Společné pro běžné RT i Path Tracing. Projeví se po restartu hry.',
 'hu': 'Az egy kötegben felépített ray tracing geometriastruktúrák legnagyobb száma (natív 256; maga a motor 128-ra áll vissza). A kisebb kötegek több képkockára osztják a munkát, ami csökkentheti a kiugrásokat új területek betöltésekor, de az újonnan betöltött objektumok kicsit később kerülhetnek be az RT árnyékokba és tükröződésekbe. Közös a normál RT és a Path Tracing számára. A játék újraindítása után érvényes.',
 'tr': 'Bir partide oluşturulan en fazla ışın izleme geometri yapısı sayısı (yerel 256; motorun kendi yedek değeri 128). Daha küçük partiler işi daha fazla kareye yayar ve yeni alanlar yüklenirken sıçramaları azaltabilir, ancak yeni yüklenen nesneler RT gölgelerine ve yansımalarına biraz daha geç katılabilir. Normal RT ve Path Tracing için ortaktır. Oyun yeniden başlatıldıktan sonra etkili olur.',
 'br': 'Número máximo de estruturas de geometria de ray tracing construídas em um lote (nativo 256; o próprio motor recorre a 128). Lotes menores distribuem o trabalho por mais quadros, o que pode reduzir picos ao carregar novas áreas, mas objetos recém-carregados podem entrar um pouco mais tarde nas sombras e reflexos RT. Compartilhado por RT normal e Path Tracing. Vale após reiniciar o jogo.',
 'jp': '1 回のバッチで構築するレイトレーシングのジオメトリ構造の最大数です（ネイティブ 256、エンジン自体のフォールバックは 128）。小さくすると構築作業がより多くのフレームに分散され、新しいエリアの読み込み時のスパイクが減る場合がありますが、新しく読み込まれたオブジェクトが RT の影や反射に加わるのが少し遅れることがあります。通常の RT と Path Tracing で共有。ゲームの再起動後に有効になります。',
 'kr': '한 번의 배치로 만드는 레이 트레이싱 지오메트리 구조의 최대 개수입니다(기본 256, 엔진 자체 대체값은 128). 배치를 작게 하면 구축 작업이 더 많은 프레임에 분산되어 새 지역을 불러올 때 급등이 줄 수 있지만, 새로 불러온 오브젝트가 RT 그림자와 반사에 조금 늦게 포함될 수 있습니다. 일반 RT와 Path Tracing이 공유. 게임을 다시 시작한 뒤 적용됩니다.',
 'cn': '每批构建的光线追踪几何结构最大数量（原生 256；引擎自身的后备值为 128）。较小的批次会把构建工作分散到更多帧，可能减少加载新区域时的尖峰，但新加载的物体可能稍晚才出现在 RT 阴影和反射中。普通 RT 与 Path Tracing 共用。重启游戏后生效。',
 'zh': '每批建置的光線追蹤幾何結構最大數量（原生 256；引擎自身的後備值為 128）。較小的批次會把建置工作分散到更多影格，可能減少載入新區域時的尖峰，但新載入的物件可能稍晚才出現在 RT 陰影與反射中。一般 RT 與 Path Tracing 共用。重新啟動遊戲後生效。',
 'ar': 'أقصى عدد من هياكل هندسة تتبع الأشعة المبنية في دفعة واحدة (الأصلي 256؛ والمحرك نفسه يعود إلى 128). الدفعات الأصغر توزع العمل على إطارات أكثر وقد تقلل القفزات عند تحميل مناطق جديدة، لكن الكائنات المحملة حديثًا قد تنضم إلى ظلال وانعكاسات RT متأخرة قليلًا. مشترك بين RT العادي وPath Tracing. يسري بعد إعادة تشغيل اللعبة.',
}
T['URTX_RTGIAlphaTest'] = {
 'en': 'Texture detail (mip level) used to cut out leaves and other alpha-tested surfaces for global-illumination rays (native 5; higher is coarser). Coarser cut-outs are cheaper and may slightly improve FPS; foliage may let a little more or less bounced light through. Does not change RT foliage shadows. Normal RT only. Applies immediately.',
 'de': 'Texturdetail (Mip-Stufe), mit dem Blätter und andere Alpha-Test-Flächen für Strahlen der globalen Beleuchtung ausgeschnitten werden (nativ 5; höher ist gröber). Gröbere Ausschnitte sind günstiger und können die FPS leicht erhöhen; Vegetation lässt etwas mehr oder weniger indirektes Licht durch. Ändert keine RT-Vegetationsschatten. Nur normales RT. Wirkt sofort.',
 'fr': "Détail de texture (niveau de mip) utilisé pour découper les feuilles et autres surfaces à test alpha pour les rayons d'illumination globale (natif 5 ; plus haut = plus grossier). Des découpes plus grossières coûtent moins et peuvent légèrement augmenter les FPS ; le feuillage peut laisser passer un peu plus ou moins de lumière indirecte. Ne change pas les ombres du feuillage en RT. RT normal uniquement. S'applique immédiatement.",
 'es': 'Detalle de textura (nivel de mip) usado para recortar hojas y otras superficies con prueba alfa para los rayos de iluminación global (nativo 5; más alto es más grueso). Recortes más gruesos cuestan menos y pueden mejorar un poco los FPS; el follaje puede dejar pasar algo más o menos luz rebotada. No cambia las sombras RT del follaje. Solo RT normal. Se aplica de inmediato.',
 'esmx': 'Detalle de textura (nivel de mip) usado para recortar hojas y otras superficies con prueba alfa para los rayos de iluminación global (nativo 5; más alto es más burdo). Recortes más burdos cuestan menos y pueden mejorar un poco los FPS; el follaje puede dejar pasar un poco más o menos luz rebotada. No cambia las sombras RT del follaje. Solo RT normal. Se aplica de inmediato.',
 'it': "Dettaglio texture (livello mip) usato per ritagliare foglie e altre superfici con alpha test per i raggi d'illuminazione globale (nativo 5; più alto è più grossolano). Ritagli più grossolani costano meno e possono aumentare leggermente gli FPS; la vegetazione può lasciar passare un po' più o meno luce riflessa. Non cambia le ombre RT della vegetazione. Solo RT normale. Si applica subito.",
 'pl': 'Szczegółowość tekstur (poziom mip) używana do wycinania liści i innych powierzchni z testem alfa dla promieni oświetlenia globalnego (natywnie 5; wyżej = grubiej). Grubsze wycięcia są tańsze i mogą lekko zwiększyć FPS; roślinność może przepuszczać nieco więcej lub mniej odbitego światła. Nie zmienia cieni roślinności w RT. Tylko zwykłe RT. Działa od razu.',
 'ru': 'Детализация текстур (уровень mip) для вырезания листьев и других поверхностей с альфа-тестом в лучах глобального освещения (по умолчанию 5; выше — грубее). Более грубые вырезы дешевле и могут немного повысить FPS; листва может пропускать чуть больше или меньше отражённого света. Не меняет RT-тени листвы. Только обычный RT. Применяется сразу.',
 'ua': 'Деталізація текстур (рівень mip) для вирізання листя та інших поверхонь з альфа-тестом у променях глобального освітлення (типово 5; вище — грубіше). Грубіші вирізи дешевші й можуть трохи підвищити FPS; листя може пропускати трохи більше або менше відбитого світла. Не змінює RT-тіні листя. Лише звичайний RT. Застосовується одразу.',
 'cz': 'Detail textur (úroveň mip) pro vyřezávání listů a dalších povrchů s alfa testem u paprsků globálního osvětlení (nativně 5; vyšší = hrubší). Hrubší výřezy jsou levnější a mohou mírně zvýšit FPS; vegetace může propouštět o něco více či méně odraženého světla. Nemění RT stíny vegetace. Pouze běžné RT. Projeví se okamžitě.',
 'hu': 'A globális megvilágítás sugaraihoz a levelek és más alfa-tesztelt felületek kivágására használt textúrarészletesség (mip-szint; natív 5; nagyobb = durvább). A durvább kivágás olcsóbb, és kissé növelheti az FPS-t; a növényzet valamivel több vagy kevesebb visszavert fényt engedhet át. Nem változtatja az RT növényzetárnyékokat. Csak normál RT. Azonnal érvényes.',
 'tr': 'Küresel aydınlatma ışınları için yaprakları ve diğer alfa testli yüzeyleri kesmekte kullanılan doku ayrıntısı (mip seviyesi; yerel 5; yüksek = daha kaba). Daha kaba kesimler daha ucuzdur ve FPS\'i biraz artırabilir; bitki örtüsü biraz daha fazla veya az yansıyan ışık geçirebilir. RT bitki gölgelerini değiştirmez. Yalnızca normal RT. Hemen uygulanır.',
 'br': 'Detalhe de textura (nível de mip) usado para recortar folhas e outras superfícies com teste alfa nos raios de iluminação global (nativo 5; maior é mais grosseiro). Recortes mais grosseiros custam menos e podem melhorar um pouco o FPS; a vegetação pode deixar passar um pouco mais ou menos luz rebatida. Não muda as sombras RT da vegetação. Apenas RT normal. Aplica-se imediatamente.',
 'jp': 'グローバルイルミネーションのレイで葉などのアルファテスト面を切り抜くテクスチャ詳細度（ミップレベル、ネイティブ 5、高いほど粗い）です。粗くすると負荷が下がり FPS がわずかに向上する場合があります。植生を通る間接光が少し増減することがあります。RT の植生の影は変わりません。通常の RT のみ。すぐに反映されます。',
 'kr': '전역 조명 광선에서 나뭇잎 등 알파 테스트 표면을 잘라내는 텍스처 디테일(밉 레벨, 기본 5, 높을수록 거침)입니다. 거칠게 하면 비용이 줄어 FPS가 조금 오를 수 있으며, 식생이 반사광을 조금 더 또는 덜 통과시킬 수 있습니다. RT 식생 그림자는 바뀌지 않습니다. 일반 RT 전용. 즉시 적용됩니다.',
 'cn': '全局光照光线用于裁剪树叶等透明测试表面的纹理细节（mip 级别；原生 5；越高越粗糙）。更粗糙的裁剪开销更低，可能略微提升 FPS；植被透过的反弹光可能略有增减。不改变 RT 植被阴影。仅限普通 RT。立即生效。',
 'zh': '全域照明光線用於裁切樹葉等透明測試表面的材質細節（mip 等級；原生 5；越高越粗糙）。更粗糙的裁切負擔更低，可能略微提升 FPS；植被透過的反彈光可能略有增減。不改變 RT 植被陰影。僅限一般 RT。立即生效。',
 'ar': 'تفاصيل الخامة (مستوى mip) المستخدمة لقص الأوراق والأسطح الأخرى ذات اختبار ألفا لأشعة الإضاءة الشاملة (الأصلي 5؛ الأعلى أخشن). القص الأخشن أقل كلفة وقد يحسن FPS قليلًا؛ وقد تمرر النباتات ضوءًا مرتدًا أكثر أو أقل قليلًا. لا يغير ظلال النباتات في RT. RT العادي فقط. يُطبق فورًا.',
}
T['URTX_PTNoBlueGlow'] = {
 'en': 'Shades surfaces seen in path-traced reflections with very low-detail textures (mip 10), which removes the blue glow visible on some armour. A visual fix: reflected textures become softer. No measured FPS change. Path Tracing only. Applies immediately.',
 'de': 'Schattiert in Path-Tracing-Spiegelungen sichtbare Flächen mit sehr detailarmen Texturen (Mip 10) und entfernt so das blaue Leuchten auf manchen Rüstungen. Eine optische Korrektur: gespiegelte Texturen werden weicher. Keine gemessene FPS-Änderung. Nur Path Tracing. Wirkt sofort.',
 'fr': "Ombre les surfaces vues dans les reflets en path tracing avec des textures très peu détaillées (mip 10), ce qui supprime la lueur bleue visible sur certaines armures. Correctif visuel : les textures reflétées deviennent plus douces. Aucun changement de FPS mesuré. Path Tracing uniquement. S'applique immédiatement.",
 'es': 'Sombrea las superficies vistas en reflejos de path tracing con texturas de muy poco detalle (mip 10), lo que elimina el brillo azul visible en algunas armaduras. Corrección visual: las texturas reflejadas se suavizan. Sin cambio de FPS medido. Solo Path Tracing. Se aplica de inmediato.',
 'esmx': 'Sombrea las superficies vistas en reflejos de path tracing con texturas de muy poco detalle (mip 10), lo que elimina el brillo azul visible en algunas armaduras. Corrección visual: las texturas reflejadas se suavizan. Sin cambio de FPS medido. Solo Path Tracing. Se aplica de inmediato.',
 'it': "Ombreggia le superfici viste nei riflessi in path tracing con texture a dettaglio molto basso (mip 10), eliminando il bagliore blu visibile su alcune armature. Correzione visiva: le texture riflesse diventano più morbide. Nessuna variazione di FPS misurata. Solo Path Tracing. Si applica subito.",
 'pl': 'Cieniuje powierzchnie widoczne w odbiciach path tracingu teksturami o bardzo niskiej szczegółowości (mip 10), co usuwa niebieską poświatę widoczną na niektórych zbrojach. Poprawka wizualna: odbite tekstury stają się miększe. Brak zmierzonej zmiany FPS. Tylko Path Tracing. Działa od razu.',
 'ru': 'Затеняет поверхности в отражениях path tracing текстурами очень низкой детализации (mip 10), что убирает синее свечение на некоторых доспехах. Визуальное исправление: отражённые текстуры становятся мягче. Измеренного изменения FPS нет. Только Path Tracing. Применяется сразу.',
 'ua': 'Затінює поверхні у відображеннях path tracing текстурами дуже низької деталізації (mip 10), що прибирає синє світіння на деяких обладунках. Візуальне виправлення: відбиті текстури стають м\'якшими. Виміряної зміни FPS немає. Лише Path Tracing. Застосовується одразу.',
 'cz': 'Stínuje povrchy viditelné v odrazech path tracingu texturami s velmi nízkým detailem (mip 10), čímž odstraní modrou záři viditelnou na některých zbrojích. Vizuální oprava: odražené textury změknou. Žádná naměřená změna FPS. Pouze Path Tracing. Projeví se okamžitě.',
 'hu': 'A path tracing tükröződésekben látható felületeket nagyon alacsony részletességű textúrákkal árnyalja (mip 10), ami eltávolítja egyes páncélokon a kék fényt. Vizuális javítás: a tükrözött textúrák lágyabbak lesznek. Nincs mért FPS-változás. Csak Path Tracing. Azonnal érvényes.',
 'tr': 'Path tracing yansımalarında görülen yüzeyleri çok düşük ayrıntılı dokularla (mip 10) gölgelendirir; bu, bazı zırhlarda görülen mavi parlamayı giderir. Görsel düzeltme: yansıyan dokular yumuşar. Ölçülmüş FPS değişikliği yok. Yalnızca Path Tracing. Hemen uygulanır.',
 'br': 'Sombreia as superfícies vistas nos reflexos de path tracing com texturas de detalhe muito baixo (mip 10), o que remove o brilho azul visível em algumas armaduras. Correção visual: as texturas refletidas ficam mais suaves. Sem alteração de FPS medida. Apenas Path Tracing. Aplica-se imediatamente.',
 'jp': 'パストレーシングの反射に映る面を非常に低詳細のテクスチャ（ミップ 10）でシェーディングし、一部の鎧に見える青い光を取り除きます。見た目の修正です。反射されたテクスチャは柔らかくなります。測定された FPS の変化はありません。Path Tracing のみ。すぐに反映されます。',
 'kr': '패스 트레이싱 반사에 보이는 표면을 매우 낮은 디테일의 텍스처(밉 10)로 셰이딩하여 일부 갑옷에 보이는 파란 빛을 없앱니다. 시각적 수정이며 반사된 텍스처가 부드러워집니다. 측정된 FPS 변화는 없습니다. Path Tracing 전용. 즉시 적용됩니다.',
 'cn': '用极低细节的纹理（mip 10）着色路径追踪反射中看到的表面，从而去除部分盔甲上可见的蓝色辉光。属于视觉修正：反射中的纹理会变得更柔和。未测得 FPS 变化。仅限 Path Tracing。立即生效。',
 'zh': '以極低細節的材質（mip 10）著色路徑追蹤反射中看到的表面，從而去除部分盔甲上可見的藍色輝光。屬於視覺修正：反射中的材質會變得更柔和。未測得 FPS 變化。僅限 Path Tracing。立即生效。',
 'ar': 'يظلل الأسطح الظاهرة في انعكاسات Path Tracing بخامات منخفضة التفاصيل جدًا (mip 10)، مما يزيل التوهج الأزرق الظاهر على بعض الدروع. إصلاح بصري: تصبح الخامات المنعكسة أنعم. لا تغير مقاس في FPS. Path Tracing فقط. يُطبق فورًا.',
}
# Existing V5.2.0 tooltip extended (the "CPU: No Wait for RT Scene" finding): PT Ray-Traced Shadows = Off already
# makes the engine skip its CPU wait for the ray-traced scene.
PTSHADOWS_ADD = {
 'en': ' Off also skips the CPU wait for the ray-traced scene.',
 'de': ' Aus überspringt auch das CPU-Warten auf die Raytracing-Szene.',
 'fr': " Désactivé supprime aussi l'attente CPU de la scène en ray tracing.",
 'es': ' Desactivado también omite la espera de la CPU a la escena con ray tracing.',
 'esmx': ' Desactivado también omite la espera de la CPU a la escena con ray tracing.',
 'it': " Disattivato salta anche l'attesa della CPU per la scena in ray tracing.",
 'pl': ' Wyłączone pomija też oczekiwanie CPU na scenę ray tracingu.',
 'ru': ' Выкл. также убирает ожидание CPU сцены трассировки лучей.',
 'ua': ' Вимк. також прибирає очікування CPU сцени трасування променів.',
 'cz': ' Vypnuto také vynechá čekání CPU na scénu ray tracingu.',
 'hu': ' A Ki a CPU várakozását is kihagyja a ray tracing jelenetre.',
 'tr': ' Kapalı, CPU\'nun ışın izlemeli sahneyi beklemesini de atlar.',
 'br': ' Desligado também elimina a espera da CPU pela cena com ray tracing.',
 'jp': 'オフではレイトレース シーンを待つ CPU の待機も省略されます。',
 'kr': ' 끄기는 레이 트레이싱 장면을 기다리는 CPU 대기도 건너뜁니다.',
 'cn': '关闭时还会跳过 CPU 对光线追踪场景的等待。',
 'zh': '關閉時還會略過 CPU 對光線追蹤場景的等待。',
 'ar': ' الإيقاف يتخطى أيضًا انتظار CPU لمشهد تتبع الأشعة.',
}

def records():
    """{lang: {key: text}} for every new key (labels, option_ labels, tooltips, headings, values)."""
    rows = {r['id']: r for r in S.ROWS_V53}
    out = {}
    for lang in LANGS:
        rec = {}
        for rid, r in rows.items():
            k = rid.lower()
            rec[k] = rec['option_' + k] = r['label']
            rec['tooltip_' + k] = T[rid][lang]
        for key, txt in zip(('urt53_sep_mem', 'urt53_sep_rtmem'), H[lang]):
            rec[key] = rec['option_' + key] = ' ' + txt + '\xa0'
        rec.update(VALUES)
        out[lang] = rec
    return out

def new_keys():
    return sorted(records()['en'])

def check():
    assert set(H) == set(LANGS) and set(PTSHADOWS_ADD) == set(LANGS)
    # no new heading longer than the longest approved V5.2.0 heading of the same language (+1): no clipping risk
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from v52_selected.locale_records import RECORDS
    for lang in LANGS:
        mx = max(len(RECORDS[lang][k].strip()) for k in RECORDS[lang] if k.startswith('urt52_sep_'))
        assert all(len(x) <= mx + 1 for x in H[lang]), (lang, H[lang], mx)
    for rid in (r['id'] for r in S.ROWS_V53):
        assert set(T[rid]) == set(LANGS), rid
        for lang, t in T[rid].items():
            assert t.strip() == t and len(t) < 520, (rid, lang, len(t))
    labels = {l for r in S.ROWS_V53 for l in r['labels'] if l.startswith('urt53_')}
    assert labels == set(VALUES), labels ^ set(VALUES)
    return True

if __name__ == '__main__':
    check(); print('PASS V5.3 strings: %d keys x %d languages' % (len(new_keys()), len(LANGS)))
