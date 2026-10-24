# -*- coding: utf-8 -*-
"""V5.1 Normal RT performance controls: new localisation strings.
Row labels follow the V4/V5 convention (English in every language, like
"RTGI: Performance Shading Frame Budget"); values and tooltips are translated.
esmx reuses es. Tooltips avoid characters outside the game font (plain 'x', '-')."""

LABELS = {
    'cr01_rt_refl_local_lights':     'RT Reflections: Local Lights',
    'cr01_rt_shadow_local_lights':   'RT Shadows: Local Light Shadows',
    'cr01_rt_shadow_foliage_detail': 'RT Shadows: Foliage Detail',
}
NEUTRAL_VALUES = {
    'preset_value_urt_n6': '6', 'preset_value_urt_n4': '4', 'preset_value_urt_n2': '2',
}
DETAIL = {  # Reduced / Low / Very Low
 'en': ('Reduced', 'Low', 'Very Low'),
 'de': ('Reduziert', 'Niedrig', 'Sehr niedrig'),
 'fr': ('Réduit', 'Bas', 'Très bas'),
 'es': ('Reducido', 'Bajo', 'Muy bajo'),
 'br': ('Reduzido', 'Baixo', 'Muito baixo'),
 'it': ('Ridotto', 'Basso', 'Molto basso'),
 'pl': ('Zmniejszony', 'Niski', 'Bardzo niski'),
 'cz': ('Snížený', 'Nízký', 'Velmi nízký'),
 'hu': ('Csökkentett', 'Alacsony', 'Nagyon alacsony'),
 'ru': ('Пониженное', 'Низкое', 'Очень низкое'),
 'ua': ('Знижене', 'Низьке', 'Дуже низьке'),
 'tr': ('Azaltılmış', 'Düşük', 'Çok düşük'),
 'jp': ('やや低', '低', '最低'),
 'kr': ('감소', '낮음', '매우 낮음'),
 'cn': ('降低', '低', '极低'),
 'zh': ('降低', '低', '極低'),
 'ar': ('مخفّض', 'منخفض', 'منخفض جدًا'),
}
TIPS = {

 'cr01_rt_refl_local_lights': {
  'en': "Normal RT, RT Reflections. Off ignores lamps, torches and fires when lighting what reflections show. Can improve FPS in towns and at night; reflections of lit areas look darker. Applies live. For original behavior, select Native / Auto.",
  'de': "Normal RT, RT-Reflexionen. Aus ignoriert Lampen, Fackeln und Feuer bei der Beleuchtung gespiegelter Flächen. Kann die FPS erhöhen in Städten und nachts; Spiegelungen beleuchteter Bereiche wirken dunkler. Wirkt sofort. Für das Originalverhalten Native / Auto wählen.",
  'fr': "Normal RT, réflexions RT. Off ignore lampes, torches et feux pour éclairer ce que montrent les réflexions. Peut améliorer les FPS en ville et la nuit ; les reflets des zones éclairées paraissent plus sombres. S'applique en direct. Pour le comportement d'origine, choisissez Native / Auto.",
  'es': "Normal RT, reflejos RT. Off ignora lámparas, antorchas y fuegos al iluminar lo que muestran los reflejos. Puede mejorar los FPS en ciudades y de noche; los reflejos de zonas iluminadas se ven más oscuros. Se aplica al instante. Para el comportamiento original, elige Native / Auto.",
  'br': "Normal RT, reflexos RT. Off ignora lâmpadas, tochas e fogueiras ao iluminar o que os reflexos mostram. Pode melhorar o FPS em cidades e à noite; reflexos de áreas iluminadas ficam mais escuros. Aplica-se na hora. Para o comportamento original, selecione Native / Auto.",
  'it': "Normal RT, riflessi RT. Off ignora lampade, torce e fuochi nell'illuminare ciò che mostrano i riflessi. Può migliorare gli FPS in città e di notte; i riflessi delle zone illuminate appaiono più scuri. Si applica subito. Per il comportamento originale, scegli Native / Auto.",
  'pl': "Normal RT, odbicia RT. Off pomija lampy, pochodnie i ogniska przy oświetlaniu tego, co widać w odbiciach. Może zwiększyć FPS w miastach i nocą; odbicia oświetlonych miejsc są ciemniejsze. Działa od razu. Aby przywrócić oryginał, wybierz Native / Auto.",
  'cz': "Normal RT, RT odrazy. Off ignoruje lampy, pochodně a ohně při osvětlení toho, co odrazy ukazují. Může zvýšit FPS ve městech a v noci; odrazy osvětlených míst jsou tmavší. Projeví se ihned. Pro původní chování zvolte Native / Auto.",
  'hu': "Normal RT, RT tükröződések. Az Off kihagyja a lámpákat, fáklyákat és tüzeket a tükröződésekben látható felületek megvilágításából. Növelheti az FPS-t városban és éjjel; a megvilágított részek tükörképe sötétebb. Azonnal érvényes. Az eredeti működéshez válaszd a Native / Auto lehetőséget.",
  'ru': "Normal RT, RT-отражения. Off не учитывает лампы, факелы и костры при освещении того, что видно в отражениях. Может повысить FPS в городах и ночью; отражения освещённых мест темнее. Применяется сразу. Для исходного поведения выберите Native / Auto.",
  'ua': "Normal RT, RT-відображення. Off не враховує лампи, смолоскипи та вогнища під час освітлення того, що видно у відображеннях. Може підвищити FPS у містах і вночі; відображення освітлених місць темніші. Застосовується одразу. Для початкової поведінки виберіть Native / Auto.",
  'tr': "Normal RT, RT yansımaları. Off, yansımalarda görünenleri aydınlatırken lambaları, meşaleleri ve ateşleri yok sayar. Şehirlerde ve gece FPS'i artırabilir; aydınlık alanların yansımaları daha karanlık görünür. Anında uygulanır. Orijinal davranış için Native / Auto seçin.",
  'jp': "Normal RT、RT 反射。Off では反射に映る部分の照明でランプ・松明・焚き火を無視します。街中や夜間に FPS が向上する場合があり、明るい場所の反射は暗く見えます。即時反映。元の動作は Native / Auto を選択。",
  'kr': "Normal RT, RT 반사. Off는 반사에 보이는 부분을 비출 때 램프·횃불·불을 무시합니다. 마을과 밤에 FPS가 향상될 수 있음; 밝은 곳의 반사가 더 어둡게 보입니다. 즉시 적용. 원래 동작은 Native / Auto를 선택하세요.",
  'cn': "Normal RT，RT 反射。关闭后，照亮反射中所见内容时忽略灯、火把和篝火。城镇和夜间 可提升 FPS；被照亮区域的反射会更暗。即时生效。如需原始效果，请选择 Native / Auto。",
  'zh': "Normal RT，RT 反射。關閉後，照亮反射中所見內容時忽略燈、火把與篝火。城鎮與夜間 可提升 FPS；被照亮區域的反射會較暗。即時生效。如需原始效果，請選擇 Native / Auto。",
  'ar': "Normal RT، انعكاسات RT. يتجاهل Off المصابيح والمشاعل والنيران عند إضاءة ما تُظهره الانعكاسات. قد يزيد عدد الإطارات في المدن وليلًا؛ تبدو انعكاسات الأماكن المضاءة أغمق. يُطبَّق فورًا. للسلوك الأصلي اختر Native / Auto.",
 },
 'cr01_rt_shadow_local_lights': {
  'en': "Normal RT, RT Shadows. Limits how many nearby lamps and fires cast ray-traced shadows (native 8; Off = none). Can improve FPS in towns, interiors and at night; fewer shadows from small lights. Sun shadows are unaffected. Applies live. For original behavior, select Native / Auto.",
  'de': "Normal RT, RT-Schatten. Begrenzt, wie viele nahe Lampen und Feuer raytracing-Schatten werfen (nativ 8; Aus = keine). Kann die FPS erhöhen in Städten, Innenräumen und nachts; weniger Schatten kleiner Lichter. Sonnenschatten bleiben unverändert. Wirkt sofort. Für das Originalverhalten Native / Auto wählen.",
  'fr': "Normal RT, ombres RT. Limite le nombre de lampes et feux proches projetant des ombres ray tracing (natif 8 ; Off = aucune). Peut améliorer les FPS en ville, en intérieur et la nuit ; moins d'ombres des petites lumières. Les ombres du soleil ne changent pas. S'applique en direct. Pour le comportement d'origine, choisissez Native / Auto.",
  'es': "Normal RT, sombras RT. Limita cuántas lámparas y fuegos cercanos proyectan sombras con trazado de rayos (nativo 8; Off = ninguna). Puede mejorar los FPS en ciudades, interiores y de noche; menos sombras de luces pequeñas. Las sombras del sol no cambian. Se aplica al instante. Para el comportamiento original, elige Native / Auto.",
  'br': "Normal RT, sombras RT. Limita quantas lâmpadas e fogueiras próximas projetam sombras com ray tracing (nativo 8; Off = nenhuma). Pode melhorar o FPS em cidades, interiores e à noite; menos sombras de luzes pequenas. Sombras do sol não mudam. Aplica-se na hora. Para o comportamento original, selecione Native / Auto.",
  'it': "Normal RT, ombre RT. Limita quante lampade e fuochi vicini proiettano ombre in ray tracing (nativo 8; Off = nessuna). Può migliorare gli FPS in città, al chiuso e di notte; meno ombre dalle luci piccole. Le ombre del sole non cambiano. Si applica subito. Per il comportamento originale, scegli Native / Auto.",
  'pl': "Normal RT, cienie RT. Ogranicza, ile pobliskich lamp i ognisk rzuca cienie ray tracing (natywnie 8; Off = żadne). Może zwiększyć FPS w miastach, wnętrzach i nocą; mniej cieni od małych świateł. Cienie słońca bez zmian. Działa od razu. Aby przywrócić oryginał, wybierz Native / Auto.",
  'cz': "Normal RT, RT stíny. Omezuje, kolik blízkých lamp a ohňů vrhá stíny ray tracingem (nativně 8; Off = žádné). Může zvýšit FPS ve městech, interiérech a v noci; méně stínů od malých světel. Stíny slunce se nemění. Projeví se ihned. Pro původní chování zvolte Native / Auto.",
  'hu': "Normal RT, RT árnyékok. Korlátozza, hány közeli lámpa és tűz vet sugárkövetett árnyékot (natív 8; Off = egy sem). Növelheti az FPS-t városban, beltérben és éjjel; kevesebb árnyék a kis fényektől. A nap árnyékai nem változnak. Azonnal érvényes. Az eredeti működéshez válaszd a Native / Auto lehetőséget.",
  'ru': "Normal RT, RT-тени. Ограничивает число ближних ламп и костров, отбрасывающих трассированные тени (по умолчанию 8; Off = нет). Может повысить FPS в городах, помещениях и ночью; меньше теней от малых источников. Тени от солнца не меняются. Применяется сразу. Для исходного поведения выберите Native / Auto.",
  'ua': "Normal RT, RT-тіні. Обмежує кількість ближніх ламп і вогнищ, що відкидають трасовані тіні (типово 8; Off = жодних). Може підвищити FPS у містах, приміщеннях і вночі; менше тіней від малих джерел. Тіні від сонця не змінюються. Застосовується одразу. Для початкової поведінки виберіть Native / Auto.",
  'tr': "Normal RT, RT gölgeleri. Yakındaki kaç lamba ve ateşin ışın izlemeli gölge düşüreceğini sınırlar (yerel 8; Off = hiç). Şehirlerde, iç mekânlarda ve gece FPS'i artırabilir; küçük ışıklardan daha az gölge. Güneş gölgeleri etkilenmez. Anında uygulanır. Orijinal davranış için Native / Auto seçin.",
  'jp': "Normal RT、RT シャドウ。レイトレース影を落とす近くのランプや炎の数を制限します（ネイティブ 8、Off = なし）。街・屋内・夜間で FPS が向上する場合があり、小さな光源の影が減ります。太陽の影は変わりません。即時反映。元の動作は Native / Auto を選択。",
  'kr': "Normal RT, RT 그림자. 레이 트레이싱 그림자를 드리우는 주변 램프와 불의 수를 제한합니다(기본 8; Off = 없음). 마을, 실내, 밤에 FPS가 향상될 수 있음; 작은 광원의 그림자가 줄어듭니다. 태양 그림자는 변하지 않습니다. 즉시 적용. 원래 동작은 Native / Auto를 선택하세요.",
  'cn': "Normal RT，RT 阴影。限制投射光线追踪阴影的附近灯光和火焰数量（原生 8；关闭 = 无）。城镇、室内和夜间 可提升 FPS；小光源的阴影减少。太阳阴影不受影响。即时生效。如需原始效果，请选择 Native / Auto。",
  'zh': "Normal RT，RT 陰影。限制投射光線追蹤陰影的附近燈光與火焰數量（原生 8；關閉 = 無）。城鎮、室內與夜間 可提升 FPS；小光源的陰影減少。太陽陰影不受影響。即時生效。如需原始效果，請選擇 Native / Auto。",
  'ar': "Normal RT، ظلال RT. يحدّ من عدد المصابيح والنيران القريبة التي تُلقي ظلالًا بتتبع الأشعة (الأصلي 8؛ Off = لا شيء). قد يزيد عدد الإطارات في المدن والأماكن المغلقة وليلًا؛ ظلال أقل من الأضواء الصغيرة. لا تتأثر ظلال الشمس. يُطبَّق فورًا. للسلوك الأصلي اختر Native / Auto.",
 },
 'cr01_rt_shadow_foliage_detail': {
  'en': "Normal RT, RT Shadows. Uses coarser leaf textures when shadow rays pass through foliage (native mip 2; Reduced 3, Low 4, Very Low 6). Can improve FPS in forests; softer, less detailed leaf shadows. Applies live. For original behavior, select Native / Auto.",
  'de': "Normal RT, RT-Schatten. Nutzt gröbere Blatttexturen, wenn Schattenstrahlen Laub durchqueren (nativ Mip 2; Reduziert 3, Niedrig 4, Sehr niedrig 6). Kann die FPS erhöhen in Wäldern; weichere, weniger detaillierte Blattschatten. Wirkt sofort. Für das Originalverhalten Native / Auto wählen.",
  'fr': "Normal RT, ombres RT. Utilise des textures de feuilles plus grossières quand les rayons d'ombre traversent le feuillage (natif mip 2 ; Réduit 3, Bas 4, Très bas 6). Peut améliorer les FPS en forêt ; ombres de feuilles plus douces et moins détaillées. S'applique en direct. Pour le comportement d'origine, choisissez Native / Auto.",
  'es': "Normal RT, sombras RT. Usa texturas de hojas más toscas cuando los rayos de sombra atraviesan el follaje (nativo mip 2; Reducido 3, Bajo 4, Muy bajo 6). Puede mejorar los FPS en bosques; sombras de hojas más suaves y menos detalladas. Se aplica al instante. Para el comportamiento original, elige Native / Auto.",
  'br': "Normal RT, sombras RT. Usa texturas de folhas mais grosseiras quando os raios de sombra atravessam a folhagem (nativo mip 2; Reduzido 3, Baixo 4, Muito baixo 6). Pode melhorar o FPS em florestas; sombras de folhas mais suaves e menos detalhadas. Aplica-se na hora. Para o comportamento original, selecione Native / Auto.",
  'it': "Normal RT, ombre RT. Usa texture delle foglie più grossolane quando i raggi d'ombra attraversano il fogliame (nativo mip 2; Ridotto 3, Basso 4, Molto basso 6). Può migliorare gli FPS nei boschi; ombre delle foglie più morbide e meno dettagliate. Si applica subito. Per il comportamento originale, scegli Native / Auto.",
  'pl': "Normal RT, cienie RT. Używa grubszych tekstur liści, gdy promienie cienia przechodzą przez listowie (natywnie mip 2; Zmniejszony 3, Niski 4, Bardzo niski 6). Może zwiększyć FPS w lasach; miększe, mniej szczegółowe cienie liści. Działa od razu. Aby przywrócić oryginał, wybierz Native / Auto.",
  'cz': "Normal RT, RT stíny. Používá hrubší textury listů, když stínové paprsky procházejí listím (nativně mip 2; Snížený 3, Nízký 4, Velmi nízký 6). Může zvýšit FPS v lesích; měkčí, méně detailní stíny listí. Projeví se ihned. Pro původní chování zvolte Native / Auto.",
  'hu': "Normal RT, RT árnyékok. Durvább levéltextúrákat használ, amikor az árnyéksugarak lombon haladnak át (natív mip 2; Csökkentett 3, Alacsony 4, Nagyon alacsony 6). Növelheti az FPS-t erdőben; lágyabb, kevésbé részletes levélárnyékok. Azonnal érvényes. Az eredeti működéshez válaszd a Native / Auto lehetőséget.",
  'ru': "Normal RT, RT-тени. Использует более грубые текстуры листвы, когда теневые лучи проходят сквозь неё (по умолчанию mip 2; Пониженное 3, Низкое 4, Очень низкое 6). Может повысить FPS в лесах; более мягкие и менее детальные тени листьев. Применяется сразу. Для исходного поведения выберите Native / Auto.",
  'ua': "Normal RT, RT-тіні. Використовує грубіші текстури листя, коли тіньові промені проходять крізь нього (типово mip 2; Знижене 3, Низьке 4, Дуже низьке 6). Може підвищити FPS у лісах; м'якші й менш детальні тіні листя. Застосовується одразу. Для початкової поведінки виберіть Native / Auto.",
  'tr': "Normal RT, RT gölgeleri. Gölge ışınları yapraklardan geçerken daha kaba yaprak dokuları kullanır (yerel mip 2; Azaltılmış 3, Düşük 4, Çok düşük 6). Ormanlarda FPS'i artırabilir; daha yumuşak, daha az ayrıntılı yaprak gölgeleri. Anında uygulanır. Orijinal davranış için Native / Auto seçin.",
  'jp': "Normal RT、RT シャドウ。影のレイが葉を通過する際に粗い葉テクスチャを使用します（ネイティブ mip 2、やや低 3、低 4、最低 6）。森で FPS が向上する場合があり、葉の影は柔らかく細部が減ります。即時反映。元の動作は Native / Auto を選択。",
  'kr': "Normal RT, RT 그림자. 그림자 광선이 잎을 통과할 때 더 거친 잎 텍스처를 사용합니다(기본 mip 2; 감소 3, 낮음 4, 매우 낮음 6). 숲에서 FPS가 향상될 수 있음; 잎 그림자가 더 부드럽고 덜 세밀해집니다. 즉시 적용. 원래 동작은 Native / Auto를 선택하세요.",
  'cn': "Normal RT，RT 阴影。阴影光线穿过树叶时使用更粗糙的叶片纹理（原生 mip 2；降低 3，低 4，极低 6）。森林中 可提升 FPS；树叶阴影更柔和、细节更少。即时生效。如需原始效果，请选择 Native / Auto。",
  'zh': "Normal RT，RT 陰影。陰影光線穿過樹葉時使用較粗糙的葉片材質（原生 mip 2；降低 3，低 4，極低 6）。森林中 可提升 FPS；樹葉陰影較柔和、細節較少。即時生效。如需原始效果，請選擇 Native / Auto。",
  'ar': "Normal RT، ظلال RT. يستخدم نسيج أوراق أخشن عندما تمر أشعة الظل عبر أوراق الشجر (الأصلي mip 2؛ مخفّض 3، منخفض 4، منخفض جدًا 6). قد يزيد عدد الإطارات في الغابات؛ ظلال أوراق أنعم وأقل تفصيلًا. يُطبَّق فورًا. للسلوك الأصلي اختر Native / Auto.",
 },
}
for _t in TIPS.values():
    _t['esmx'] = _t['es']
DETAIL['esmx'] = DETAIL['es']

def new_strings(lang):
    """All new keys -> text for one language (deterministic order)."""
    d = {}
    for k, v in LABELS.items():
        d[k] = v
        d['option_' + k] = v      # the options menu draws the row label from option_<displayName>
    d.update(NEUTRAL_VALUES)
    r, l, vl = DETAIL[lang]
    d['preset_value_urt_reduced'] = r
    d['preset_value_urt_low'] = l
    d['preset_value_urt_verylow'] = vl
    for k, t in TIPS.items():
        d['tooltip_' + k] = t[lang]
    return d
