# -*- coding: utf-8 -*-
"""V5.3.0 EXPERIMENTAL section: description cores (18 languages). Assembled by tooltips_dev2.tooltips().
R11: the R6-R10 rows were retired (see tools/v52_selected/spec.py RETIRED_R11); history in tags v5.3.0-private-r7 .. r10."""
CORE = {}
# R12 FIX3: PT: Second Bounce Coverage retired (history: tag v5.3.0-private-r11 .. r12-fix2); PT: Bounce Sun Shadow Coverage
# moved to the PT section (CORE_PT below). R12 FIX4: PT: Bounce Local Light Shadow Coverage moved there too; no EXPERIMENTAL rows remain.

# R12 FIX2: no longer EXPERIMENTAL (beneath PT: Geometry Bounces): no leading "Experimental." sentence to drop.
CORE_PT = {}
CORE_PT['URTX_PTDiffuseCoverage'] = {
 'en': "Reduces first-bounce diffuse sampling (moving, weighted pattern): less Path Tracing work. May add indirect-light noise. 50% is recommended alongside PT: Geometry Bounces set to 1 for maximum FPS in our tested configuration.",
 'de': "Reduziert das diffuse Sampling des ersten Abpralls auf diesen Pixelanteil pro Frame (wanderndes, gewichtetes Muster): weniger Path-Tracing-Arbeit. Streulicht kann rauschen. 50% empfohlen mit PT: Geometry Bounces 1 für maximale FPS in unserem Test-Setup.",
 'fr': "Réduit l'échantillonnage diffus du 1er rebond à cette part des pixels par image (motif mobile pondéré) : moins de travail de Path Tracing. Peut bruiter la lumière indirecte. 50% recommandé avec PT: Geometry Bounces 1 pour un FPS maximal dans notre test.",
 'es': "Reduce el muestreo difuso del 1er rebote a esta parte de los píxeles por fotograma (patrón móvil ponderado): menos trabajo de Path Tracing. Puede añadir ruido indirecto. 50% recomendado con PT: Geometry Bounces 1 para el máximo de FPS en nuestra prueba.",
 'esmx': "Reduce el muestreo difuso del 1er rebote a esta parte de los píxeles por cuadro (patrón móvil ponderado): menos trabajo de Path Tracing. Puede añadir ruido indirecto. 50% recomendado con PT: Geometry Bounces 1 para el máximo de FPS en la prueba.",
 'it': "Riduce il campionamento diffuso del 1° rimbalzo a questa quota di pixel per fotogramma (schema mobile pesato): meno lavoro di Path Tracing. Può aggiungere rumore indiretto. 50% consigliato con PT: Geometry Bounces 1 per il massimo degli FPS nel nostro test.",
 'pl': "Ogranicza rozproszone próbkowanie 1. odbicia do tej części pikseli na klatkę (ruchomy, ważony wzór): mniej pracy Path Tracingu. Może dodać szum pośredni. 50% zalecane z PT: Geometry Bounces 1 dla maksimum FPS w naszym teście.",
 'ru': "Сокращает рассеянную выборку 1-го отскока до этой доли пикселей за кадр (сдвигающийся взвешенный узор): меньше работы Path Tracing. Возможен шум непрямого света. 50% рекомендуется с PT: Geometry Bounces 1 для максимума FPS в нашем тесте.",
 'ua': "Скорочує розсіяну вибірку 1-го відскоку до цієї частки пікселів за кадр (зсувний зважений візерунок): менше роботи Path Tracing. Можливий шум непрямого світла. 50% рекомендовано з PT: Geometry Bounces 1 для максимуму FPS у нашому тесті.",
 'cz': "Omezí difuzní vzorkování 1. odrazu na tento podíl pixelů za snímek (posouvaný vážený vzor): méně práce Path Tracingu. Může přidat šum nepřímého světla. 50% doporučeno s PT: Geometry Bounces 1 pro maximum FPS v našem testu.",
 'hu': "Az első visszaverődés szórt mintavételét a pixelek ezen részére csökkenti képkockánként (mozgó, súlyozott minta): kevesebb Path Tracing munka. Zajosabb lehet a közvetett fény. 50% ajánlott PT: Geometry Bounces 1 mellett a legtöbb FPS-hez a tesztünkben.",
 'tr': "İlk sekmenin dağınık örneklemesini her karede piksellerin bu kısmına indirir (kayan, ağırlıklı desen): daha az Path Tracing işi. Dolaylı ışıkta gürültü artabilir. Testimizde en yüksek FPS için PT: Geometry Bounces 1 ile 50% önerilir.",
 'br': "Reduz a amostragem difusa do 1º quique a esta parte dos pixels por quadro (padrão móvel ponderado): menos trabalho de Path Tracing. Pode adicionar ruído indireto. 50% recomendado com PT: Geometry Bounces 1 para o máximo de FPS no nosso teste.",
 'jp': "1 回目のバウンスの拡散サンプリングを毎フレームこの割合のピクセルに減らし（移動・重み付きパターン）、Path Tracing の処理を減らします。間接光のノイズが増える場合があります。検証構成で最大 FPS には PT: Geometry Bounces 1 と 50% を推奨します。",
 'kr': "첫 바운스의 확산 샘플링을 매 프레임 이 비율의 픽셀로 줄여(이동 패턴, 가중치로 보정) Path Tracing 작업을 줄입니다. 간접광 노이즈가 늘 수 있습니다. 테스트한 구성에서 최대 FPS를 위해 PT: Geometry Bounces 1과 50%를 권장합니다.",
 'cn': "将第一次反弹的漫反射采样减少到每帧该比例的像素（移动图案，按权重补偿），使路径追踪的工作量减少。可能增加间接光噪点。在我们测试的配置中，推荐 50% 搭配 PT: Geometry Bounces 1 以获得最高 FPS。",
 'zh': "將第一次反彈的漫反射取樣減少到每幀該比例的像素（移動圖案，依權重補償），使路徑追蹤的工作量減少。可能增加間接光雜訊。在我們測試的配置中，建議 50% 搭配 PT: Geometry Bounces 1 以獲得最高 FPS。",
 'ar': "يقلل عينات الارتداد الأول المنتشرة إلى هذه النسبة من البكسلات لكل إطار (نمط متحرك مرجّح)، فيقل عمل Path Tracing. قد يزيد تشويش الإضاءة غير المباشرة. يُوصى بـ 50% مع PT: Geometry Bounces 1 لأعلى FPS في اختبارنا.",
}

CORE_PT['URTX_ExpPTSunShadowCoverage'] = {
 'en': "Sun / moon shadow visibility at bounce hits is calculated for half of the pixels per frame (weighted): less Path Tracing work. May add shadow noise or flicker. For performance: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, this 50%.",
 'de': "Sonnen-/Mondschatten-Sichtbarkeit an Abprallpunkten wird pro Frame für die Hälfte der Pixel berechnet (gewichtet): weniger Path-Tracing-Arbeit. Schattenrauschen oder Flimmern möglich. Für Leistung: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, dies 50%.",
 'fr': "Visibilité de l'ombre du soleil/de la lune aux rebonds calculée pour la moitié des pixels par image (pondérée) : moins de travail de Path Tracing. Bruit ou scintillement possible. Performances : PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, ceci 50%.",
 'es': "Visibilidad de la sombra del sol/luna en rebotes calculada para la mitad de píxeles por fotograma (ponderada): menos trabajo de Path Tracing. Posible ruido o parpadeo. Rendimiento: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, esto 50%.",
 'esmx': "Visibilidad de sombra del sol/luna en rebotes calculada para la mitad de píxeles por cuadro (ponderada): menos trabajo de Path Tracing. Posible ruido o parpadeo. Rendimiento: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, esto 50%.",
 'it': "La visibilità dell'ombra di sole/luna nei rimbalzi è calcolata per metà dei pixel a fotogramma (pesata): meno lavoro di Path Tracing. Possibili rumore o sfarfallio. Prestazioni: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, questo 50%.",
 'pl': "Widoczność cienia słońca/księżyca w punktach odbicia jest liczona dla połowy pikseli na klatkę (ważona): mniej pracy Path Tracingu. Możliwy szum lub migotanie cieni. Wydajność: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, to 50%.",
 'ru': "Видимость тени солнца/луны в точках отскока считается для половины пикселей за кадр (взвешенно): меньше работы Path Tracing. Возможны шум или мерцание теней. Для FPS: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, это 50%.",
 'ua': "Рахує видимість тіні сонця/місяця в точках відскоку для половини пікселів за кадр (зсувний зважений візерунок): менше роботи Path Tracing. Можливі шум або мерехтіння тіней. Для продуктивності: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50% і це на 50%.",
 'cz': "Viditelnost stínu slunce/měsíce v bodech odrazu se počítá u poloviny pixelů za snímek (váženě): méně práce Path Tracingu. Možný šum nebo blikání stínů. Pro výkon doporučujeme PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50% a toto na 50%.",
 'hu': "A nap/hold árnyékának láthatóságát a visszaverődési pontokon képkockánként a pixelek felénél számolja (súlyozva): kevesebb Path Tracing munka. Árnyékzaj vagy villogás lehet. Teljesítményhez: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, ez 50%.",
 'tr': "Sekme noktalarında güneş/ay gölgesi görünürlüğü her karede piksellerin yarısı için hesaplanır (ağırlıklı): daha az Path Tracing işi. Gölge gürültüsü veya titreme olabilir. Performans: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, bu 50%.",
 'br': "A visibilidade da sombra do sol/lua nos quiques é calculada para metade dos pixels por quadro (ponderada): menos trabalho de Path Tracing. Pode haver ruído ou cintilação. Desempenho: PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, isto 50%.",
 'jp': "バウンス点の太陽／月の影の可視性を毎フレーム半分のピクセルで計算（重み付き）し、Path Tracing の処理を減らします。影のノイズやちらつきが出る場合があります。性能重視: PT: Geometry Bounces 1、First Bounce Diffuse Coverage 50%、本設定 50%。",
 'kr': "바운스 지점의 해/달 그림자 가시성 계산을 매 프레임 절반의 픽셀로 줄여(이동 패턴, 가중치로 보정) Path Tracing 작업을 줄입니다. 그림자 노이즈나 깜빡임이 생길 수 있습니다. 성능을 위해 PT: Geometry Bounces 1, First Bounce Diffuse Coverage 50%, 이 설정 50%를 권장합니다.",
 'cn': "在反弹点每帧只为一半像素计算太阳或月亮阴影的可见性（移动图案，按权重补偿），使路径追踪的工作量减少。阴影可能出现噪点或闪烁。追求性能时推荐 PT: Geometry Bounces 1、First Bounce Diffuse Coverage 50%，本项 50%。",
 'zh': "在反彈點每幀只為一半像素計算太陽或月亮陰影的可見性（移動圖案，依權重補償），使路徑追蹤的工作量減少。陰影可能出現雜訊或閃爍。追求效能時建議 PT: Geometry Bounces 1、First Bounce Diffuse Coverage 50%，本項 50%。",
 'ar': "يحسب رؤية ظل الشمس أو القمر عند نقاط الارتداد لنصف البكسلات لكل إطار (نمط متحرك مرجّح)، فيقل عمل Path Tracing. قد يظهر تشويش أو وميض في الظلال. للأداء نوصي بـ PT: Geometry Bounces 1 وFirst Bounce Diffuse Coverage 50% وهذا الخيار على 50%.",
}

CORE_PT['URTX_ExpPTLocalShadowCoverage'] = {
 'en': "Local-light shadow visibility at bounce hits is calculated for half of the pixels per frame (weighted): less Path Tracing work. Helps most in scenes with lamps, torches or fires. May add lighting noise or flicker.",
 'de': "Die Schatten-Sichtbarkeit lokaler Lichter an Abprallpunkten wird pro Frame für die Hälfte der Pixel berechnet (gewichtet): weniger Path-Tracing-Arbeit. Hilft vor allem bei Lampen, Fackeln oder Feuer. Rauschen oder Flimmern möglich.",
 'fr': "La visibilité des ombres des lumières locales aux rebonds est calculée pour la moitié des pixels par image (pondérée) : moins de travail de Path Tracing. Utile surtout avec lampes, torches ou feux. Bruit ou scintillement possible.",
 'es': "La visibilidad de sombra de las luces locales en los rebotes se calcula para la mitad de los píxeles por fotograma (ponderada): menos trabajo de Path Tracing. Ayuda sobre todo con lámparas, antorchas o fuego. Puede haber ruido o parpadeo.",
 'esmx': "La visibilidad de sombra de las luces locales en los rebotes se calcula para la mitad de los píxeles por cuadro (ponderada): menos trabajo de Path Tracing. Ayuda sobre todo con lámparas, antorchas o fuego. Puede haber ruido o parpadeo.",
 'it': "La visibilità delle ombre delle luci locali nei rimbalzi è calcolata per metà dei pixel a fotogramma (pesata): meno lavoro di Path Tracing. Aiuta soprattutto con lampade, torce o fuochi. Possibili rumore o sfarfallio.",
 'pl': "Widoczność cieni świateł lokalnych w punktach odbicia jest liczona dla połowy pikseli na klatkę (ważona): mniej pracy Path Tracingu. Pomaga głównie przy lampach, pochodniach lub ogniu. Możliwy szum lub migotanie.",
 'ru': "Видимость теней локальных источников в точках отскока считается для половины пикселей за кадр (взвешенно): меньше работы Path Tracing. Полезно при лампах, факелах или огне. Возможны шум или мерцание.",
 'ua': "Видимість тіней локальних джерел у точках відскоку рахується для половини пікселів за кадр (зважено): менше роботи Path Tracing. Корисно при лампах, смолоскипах чи вогні. Можливі шум або мерехтіння.",
 'cz': "Viditelnost stínů lokálních světel v bodech odrazu se počítá u poloviny pixelů za snímek (váženě): méně práce Path Tracingu. Pomáhá hlavně u lamp, pochodní či ohně. Možný šum nebo blikání.",
 'hu': "A helyi fények árnyékának láthatóságát a visszaverődési pontokon képkockánként a pixelek felénél számolja (súlyozva): kevesebb Path Tracing munka. Lámpáknál, fáklyáknál, tűznél segít. Zaj vagy villogás lehet.",
 'tr': "Sekme noktalarında yerel ışık gölgesi görünürlüğü her karede piksellerin yarısı için hesaplanır (ağırlıklı): daha az Path Tracing işi. En çok lamba, meşale veya ateşli sahnelerde işe yarar. Gürültü veya titreme olabilir.",
 'br': "A visibilidade da sombra das luzes locais nos quiques é calculada para metade dos pixels por quadro (ponderada): menos trabalho de Path Tracing. Ajuda mais com lâmpadas, tochas ou fogo. Pode haver ruído ou cintilação.",
 'jp': "バウンス点のローカルライトの影の可視性を毎フレーム半分のピクセルで計算（重み付き）し、Path Tracing の処理を減らします。ランプ・松明・火のある場面で特に有効です。ノイズやちらつきが出る場合があります。",
 'kr': "바운스 지점의 로컬 광원 그림자 가시성을 매 프레임 절반의 픽셀에서 계산(가중치 보정)해 Path Tracing 작업을 줄입니다. 램프, 횃불, 불이 있는 장면에서 가장 효과적입니다. 노이즈나 깜빡임이 생길 수 있습니다.",
 'cn': "在反弹点每帧只为一半像素计算局部光源阴影的可见性（按权重补偿），使路径追踪的工作量减少。在有灯、火把或火焰的场景中最有效。可能出现噪点或闪烁。",
 'zh': "在反彈點每幀只為一半像素計算局部光源陰影的可見性（依權重補償），使路徑追蹤的工作量減少。在有燈、火把或火焰的場景中最有效。可能出現雜訊或閃爍。",
 'ar': "تُحسب رؤية ظلال الأضواء المحلية عند نقاط الارتداد لنصف البكسلات لكل إطار (مرجّحة)، فيقل عمل Path Tracing. يفيد أكثر في المشاهد ذات المصابيح أو المشاعل أو النار. قد يظهر تشويش أو وميض.",
}

# The EXPERIMENTAL heading already says so: drop the leading one-word "Experimental." sentence (saves tooltip width).
import re as _re
for _k in CORE:
    CORE[_k] = {l: _re.split(r'(?<=[.。])\s*', t, maxsplit=1)[1] for l, t in CORE[_k].items()}
CORE.update(CORE_PT)   # R12 FIX2: added after the EXPERIMENTAL rows' first sentence was dropped
