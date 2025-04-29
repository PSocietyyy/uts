<?php
// Bikin array data mahasiswa
$mahasiswa = [
    [
        'nama' => 'Ahmad Rizky',
        'tugas' => 80,
        'uts' => 75,
        'uas' => 85
    ],
    [
        'nama' => 'Budiono Siregar',
        'tugas' => 70,
        'uts' => 65,
        'uas' => 60
    ],
    [
        'nama' => 'Cha Hae-In',
        'tugas' => 90,
        'uts' => 85,
        'uas' => 95
    ]
];

// Fungsi buat nentuin predikat nilai
function getPredikat($nilaiAkhir) {
    if ($nilaiAkhir >= 85 && $nilaiAkhir <= 100) {
        return 'A';
    } elseif ($nilaiAkhir >= 80) {
        return 'B+';
    } elseif ($nilaiAkhir >= 70) {
        return 'B';
    } elseif ($nilaiAkhir >= 60) {
        return 'C+';
    } elseif ($nilaiAkhir >= 50) {
        return 'C';
    } elseif ($nilaiAkhir >= 40) {
        return 'D';
    } else {
        return 'E';
    }
}

// Ngitung nilai akhir dan nentuin predikat
foreach ($mahasiswa as $data) {
    $nilaiAkhir = ($data['tugas'] * 0.3) + ($data['uts'] * 0.3) + ($data['uas'] * 0.4);
    $predikat = getPredikat($nilaiAkhir);
    
    echo "Nama Mahasiswa : " . $data['nama'] . "<br>";
    echo "Nilai Tugas    : " . $data['tugas'] . "<br>";
    echo "Nilai UTS      : " . $data['uts'] . "<br>";
    echo "Nilai UAS      : " . $data['uas'] . "<br>";
    echo "Nilai Akhir    : " . number_format($nilaiAkhir, 2) . "<br>";
    echo "Predikat       : " . $predikat . "<br><br>";
}
?>
