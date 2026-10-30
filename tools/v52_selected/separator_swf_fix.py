#!/usr/bin/env python3
"""RC7 separator repair for panel_ingamemenu.redswf (byte-size neutral, both movie copies).

Root cause (game code, W3OptionsSeparator `width` setter, ABC method 1653):
    if (label.textWidth == 0) { rightLine.visible = false; leftLine.width = gap; }
    else { ...sizes and places both lines... }        // never sets rightLine.visible = true
The separator object is created once per recycled list row and reused for every separator
row (and every list refresh). Once ANY layout of that object ran with a 0 text width (an
unnamed SUBTLE_SEPARATOR of any menu page, or a named heading measured before its text
was laid out), its right line stays hidden for every later heading shown by that row.
RC5 removed URT's own unnamed separators, which reduced but could not remove the cases
(RC6 screenshots: missing line at first opening and after one scroll step).

Fix: replace the 14 bytes at code offset 37 of that setter
    pushbyte 0 / ifne L_named / getlocal_0 getproperty rightLine pushfalse setproperty visible
with exactly 14 bytes
    dup / getlocal_0 getproperty rightLine / swap / convert_b / setproperty visible / iftrue L_named
i.e.  rightLine.visible = (textWidth != 0)  on EVERY layout, then the original branch.
Unnamed separators keep their original look (right line hidden, full-width left line);
every named heading shows its right line again, whatever the row showed before.
Same code length, same branch targets, max_stack 3 unchanged (verified below).

Both representations in the resource are patched: the zlib-compressed GFx movie (CFX block,
re-encoded as a complete zlib stream of exactly the original block size: compressed part plus
stored blocks, no padding)
and the uncompressed SWF copy that follows it. No size field, offset or other byte of the
resource changes. Usage: separator_swf_fix.py <original.redswf> <out.redswf>"""
import hashlib, struct, sys, zlib
from pathlib import Path

ORIGINAL_SHA = '9a9f9c5ac41990bc8819e0aaed8a03018fe06db69bd2a2f8648b4064aec28142'
OLD = bytes.fromhex('2400' '14140000' 'd0669609' '27' '61a901')
NEW = bytes.fromhex('2a' 'd0669609' '2b' '76' '61a901' '110c0000')
assert len(OLD) == len(NEW) == 14
HEADER = bytes.fromhex('f50c' '03' '05' '0a' '0b' '8b01')   # method 1653, max_stack 3, locals 5, scopes 10/11, code_length 139
PREFIX = bytes.fromhex('d030d06674d0667466b62e619709d1d0667466b62ea173d62405d7240f6304d0667466b62e')  # code bytes 0..36


# V5.2.0 final, part 2 - W3SubMenuListItemRenderer.initSeparator (named headings):
# original order  label.text, x, y, width (-> layout above), height, visible = true, alpha = 1
# new order       visible = true, alpha = 1, label.text, x, y, width, height
# A recycled row hides its separator object while it shows a normal option. When the row is reused for a
# heading, the original code set the title and ran the layout while the object was still hidden, then made it
# visible. The game's install shows the repaired movie is the one loaded (kit rev12 evidence) and the right
# line still vanished periodically while scrolling: the only remaining way for the layout to hide it is a
# 0 measured title width, i.e. a title measured while hidden. Showing the object before setting and measuring
# its title removes that state dependency. Same instructions, same length, no branch into the moved range
# except its first byte (offset 63), so the move is exact.
INIT_CODE = bytes.fromhex(
    'd030208050d5d0668e1720143000005da22e2ce00b46a22e016050878050d5d0d1420060940987688e175d9030d0668e175dba346'
    '0ff0846ba34014f903002d0668e17667460be0266a53461f903d0668e1725eafeffff0f61f92bd0668e17242261fb2bd0668e17d0'
    '669709619709d0668e17d0669809619809d0668e172661a901d0668e17240161892dd04f820400d04fc3090047')
SEG = {'text': (63, 78), 'x': (78, 91), 'y': (91, 100), 'width': (100, 111), 'height': (111, 122),
       'visible': (122, 130), 'alpha': (130, 139)}
NEW_ORDER = ('visible', 'alpha', 'text', 'x', 'y', 'width', 'height')


def reordered_init() -> bytes:
    c = INIT_CODE
    assert len(c) == 150 and c[11] == 0x14 and int.from_bytes(c[12:15], 'little', signed=True) + 15 == 63  # ifne -> 63
    body = b''.join(c[SEG[k][0]:SEG[k][1]] for k in NEW_ORDER)
    out = c[:63] + body + c[139:]
    assert len(out) == len(c)
    return out


# V5.2.0 release, part 3 - W3SubMenuListItemRenderer.draw (ABC method 4688): the separator is laid out ONLY in
# initSeparator, i.e. once, at setData time, from the renderer's width and the title width measured at that instant.
# Two lifecycle facts make that single layout unreliable (found in the list code of this movie):
#  * the list sizes its rows AFTER/INDEPENDENTLY of setData (ScrollingList.drawLayout: renderer.width =
#    availableWidth - padding, availableWidth = list width - margins - scroll bar), so a row laid out at one width
#    keeps that geometry when the row width changes (first opening, page switch, main menu vs in-game menu);
#  * W3ScrollingList.populateData fills every row and only then shows the list (ShowRenderers(true)); a title that
#    cannot be measured at setData time (0 px) leaves the heading with one line until the row is filled again.
# The game's draw() has a SIZE branch that can never run for these rows (BaseListItem sets preventAutosizing and
# constraintsDisabled; setActualSize is empty) and an autoSize branch whose invalidateSize() is discarded by
# validateNow (it clears the invalidation hash after draw). That space now holds:
#     if (mcSeparator && _type == IngameMenu.IGMActionType_Separator) {
#         mcSeparator.width = this.width;                       // lay out again at the CURRENT row width
#         if (!mcSeparator.label.textWidth) { addEventListener(Event.ENTER_FRAME, invalidate, false, 0, true); return; }
#     }                                                          // retry next frame (weak listener) until measurable
#     removeEventListener(Event.ENTER_FRAME, invalidate);        // any other row, or measured: no retry pending
# draw() runs on every validation of the row (setData, every resize by the list, state changes), so the lines follow
# the row's real width and a heading whose title was not measurable is laid out again on the next frame, until it is.
# Unnamed (subtle) separators and normal options are untouched. Same code length (109), max_stack 3 -> 6.
DRAW_HEADER = bytes.fromhex('d024' '03' '01' '0e' '0f' '6d')          # method 4688, max_stack 3, locals 1, scopes 14/15, 109
DRAW_CODE = bytes.fromhex(
    'd0305dc40960c80166882646c4090112190000d04f902200608a0c60c22e66ba28130700005dc5094fc509005dc40960c80166862646c40901'
    '122f000060ed0b111400005d910c4f910c005dba09609c09609d094fba090260eb0b110d000060a909609c09609d094fc5050247')
DRAW_HEADER_NEW = bytes.fromhex('d024' '06' '01' '0e' '0f' '6d')


def new_draw() -> bytes:
    c = DRAW_CODE
    assert len(c) == 109 and c[15] == 0x12 and c[19:24] == bytes.fromhex('d04f902200')   # iffalse DATA / updateText()
    head = c[:15] + bytes.fromhex('12050000') + c[19:24]        # DATA branch now skips to offset 24 (our code)
    sep = bytes.fromhex('d0668e17')                              # this.mcSeparator
    body = (sep + bytes.fromhex('123a0000')                      # 28 iffalse REMOVE(90)
            + bytes.fromhex('d066f021' '6066' '66bb2b' '142d0000')   # 32 _type, IngameMenu.IGMActionType_Separator, ifne REMOVE
            + sep + bytes.fromhex('d0669709' '619709')           # 45 mcSeparator.width = this.width
            + sep + bytes.fromhex('6674' '66b62e')               # 56 .label.textWidth
            + bytes.fromhex('11150000')                          # 65 iftrue REMOVE(90)
            + bytes.fromhex('5d8504' '600a' '66d42c' 'd0668204' '27' '2400' '26' '4f850405' '47')  # 69 addEventListener (weak); return
            + bytes.fromhex('5d8604' '600a' '66d42c' 'd0668204' '4f860402')   # 90 REMOVE: removeEventListener
            + bytes.fromhex('0202' '47'))                        # 106 return
    out = head + body
    assert len(out) == len(c), len(out)
    return out


def patch_movie(movie: bytes) -> bytes:
    i = movie.index(HEADER + PREFIX)
    assert movie.count(HEADER + PREFIX) == 1
    at = i + len(HEADER) + len(PREFIX)
    assert movie[at:at + 14] == OLD, 'unexpected separator bytecode'
    movie = movie[:at] + NEW + movie[at + 14:]
    assert movie.count(INIT_CODE) == 1, 'unexpected initSeparator bytecode'
    j = movie.index(INIT_CODE)
    movie = movie[:j] + reordered_init() + movie[j + len(INIT_CODE):]
    assert movie.count(DRAW_HEADER + DRAW_CODE) == 1, 'unexpected renderer draw bytecode'
    k = movie.index(DRAW_HEADER + DRAW_CODE)
    return movie[:k] + DRAW_HEADER_NEW + new_draw() + movie[k + len(DRAW_HEADER) + len(DRAW_CODE):]


def _stored(data: bytes) -> bytes:
    """Raw-deflate stored blocks (last one final) for `data`."""
    out = bytearray(); n = len(data); pos = 0
    while True:
        chunk = data[pos:pos + 65535]; pos += len(chunk)
        final = pos >= n
        out += bytes([1 if final else 0]) + struct.pack('<HH', len(chunk), len(chunk) ^ 0xffff) + chunk
        if final:
            return bytes(out)


def exact_zlib(data: bytes, size: int) -> bytes:
    """zlib stream (header, deflate, adler32) of exactly `size` bytes: data[:k] compressed (level 9,
    memLevel 6, ends byte-aligned via SYNC_FLUSH) + data[k:] in stored blocks. k is searched so the
    total length matches; zlib inflates it like any other stream. No trailing padding."""
    def build(k):
        co = zlib.compressobj(9, zlib.DEFLATED, -15, 6)
        body = co.compress(data[:k]) + co.flush(zlib.Z_SYNC_FLUSH)
        return b'\x78\xda' + body + _stored(data[k:]) + struct.pack('>I', zlib.adler32(data))
    lo, hi = 0, len(data)
    assert len(build(hi)) <= size <= len(build(lo)), 'cannot reach the exact size'
    while hi - lo > 64:                                        # size grows as k shrinks (more stored bytes)
        mid = (lo + hi) // 2
        if len(build(mid)) > size: lo = mid
        else: hi = mid
    for k in range(hi, max(lo, hi - 4096), -1):
        s = build(k)
        if len(s) == size:
            assert zlib.decompress(s) == data
            return s
    raise AssertionError('no exact split found')


def patch(orig: bytes) -> bytes:
    assert hashlib.sha256(orig).hexdigest() == ORIGINAL_SHA, 'not the original V5.1.0 panel_ingamemenu.redswf'
    assert orig[:4] == b'CR2W'
    c = orig.index(b'CFX')
    block = struct.unpack_from('<I', orig, c - 4)[0]          # CR2W byte-array length of the CFX block
    z = zlib.decompressobj()
    movie = z.decompress(orig[c + 8:])
    assert z.eof and c + 8 + (len(orig) - c - 8 - len(z.unused_data)) == c + block
    assert zlib.compress(movie, 9) == orig[c + 8:c + block], 'original block is zlib level 9 (reproducible)'
    new_movie = patch_movie(movie)
    room = block - 8
    comp = exact_zlib(new_movie, room)                      # a complete zlib stream of EXACTLY the original length
    out = bytearray(orig)
    out[c + 8:c + block] = comp
    f = orig.index(b'FWS', c + block)                         # uncompressed copy
    flen = struct.unpack_from('<I', orig, f + 4)[0]
    out[f:f + flen] = patch_movie(bytes(orig[f:f + flen]))
    out = bytes(out)
    assert len(out) == len(orig)
    # every byte outside the two movie bodies is unchanged
    assert out[:c + 8] == orig[:c + 8] and out[c + block:f] == orig[c + block:f] and out[f + flen:] == orig[f + flen:]
    z2 = zlib.decompressobj()
    assert z2.decompress(out[c + 8:]) == new_movie and z2.eof
    return out


if __name__ == '__main__':
    src, dst = map(Path, sys.argv[1:3])
    o = src.read_bytes(); n = patch(o)
    dst.write_bytes(n)
    print('PASS separator fix: size %d unchanged; original %s -> %s' % (len(n), hashlib.sha256(o).hexdigest()[:12], hashlib.sha256(n).hexdigest()[:12]))
