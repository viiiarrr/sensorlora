#include "fuzzy_logic.h"
#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

// ==========================================
// himpunan fuzzy
// ==========================================
// curah hujan (mm/jam)
FuzzySet *hKering  = new FuzzySet(0, 0, 1, 5);
FuzzySet *hGerimis = new FuzzySet(2, 5, 10, 15);
FuzzySet *hDeras   = new FuzzySet(10, 20, 50, 50);

// soil moisture (%)
FuzzySet *tRendah = new FuzzySet(0, 0, 30, 40);
FuzzySet *tSedang = new FuzzySet(30, 45, 55, 70);
FuzzySet *tTinggi = new FuzzySet(60, 75, 100, 100);

// pergerakan/kemiringan (derajat)
FuzzySet *pAman    = new FuzzySet(0, 0, 3, 5);
FuzzySet *pWaspada = new FuzzySet(3, 7, 10, 15);
FuzzySet *pBahaya  = new FuzzySet(10, 20, 90, 90);

// output status (0-100)
FuzzySet *sAman    = new FuzzySet(0, 0, 20, 40);
FuzzySet *sWaspada = new FuzzySet(30, 50, 50, 70);
FuzzySet *sAwas    = new FuzzySet(60, 80, 100, 100);

// penulisan rule
void tambahAturan(int id, FuzzySet *hujan, FuzzySet *tanah, FuzzySet *gerak, FuzzySet *status) {
    FuzzyRuleAntecedent *ifHujanDanTanah = new FuzzyRuleAntecedent();
    ifHujanDanTanah->joinWithAND(hujan, tanah);
    
    FuzzyRuleAntecedent *ifKondisi = new FuzzyRuleAntecedent();
    ifKondisi->joinWithAND(ifHujanDanTanah, gerak);
    
    FuzzyRuleConsequent *thenStatus = new FuzzyRuleConsequent();
    thenStatus->addOutput(status);
    
    FuzzyRule *aturan = new FuzzyRule(id, ifKondisi, thenStatus);
    fuzzy->addFuzzyRule(aturan);
}

void setupFuzzy() {
    FuzzyInput *hujan = new FuzzyInput(1);
    hujan->addFuzzySet(hKering); hujan->addFuzzySet(hGerimis); hujan->addFuzzySet(hDeras);
    fuzzy->addFuzzyInput(hujan);

    FuzzyInput *kelembapan = new FuzzyInput(2);
    kelembapan->addFuzzySet(tRendah); kelembapan->addFuzzySet(tSedang); kelembapan->addFuzzySet(tTinggi);
    fuzzy->addFuzzyInput(kelembapan);

    FuzzyInput *pergerakan = new FuzzyInput(3);
    pergerakan->addFuzzySet(pAman); pergerakan->addFuzzySet(pWaspada); pergerakan->addFuzzySet(pBahaya);
    fuzzy->addFuzzyInput(pergerakan);

    FuzzyOutput *status = new FuzzyOutput(1);
    status->addFuzzySet(sAman); status->addFuzzySet(sWaspada); status->addFuzzySet(sAwas);
    fuzzy->addFuzzyOutput(status);

    // 27 basis aturan
    // hujan kering
    tambahAturan(1, hKering, tRendah, pAman, sAman);
    tambahAturan(2, hKering, tRendah, pWaspada, sWaspada);
    tambahAturan(3, hKering, tRendah, pBahaya, sAwas);
    tambahAturan(4, hKering, tSedang, pAman, sAman);
    tambahAturan(5, hKering, tSedang, pWaspada, sWaspada);
    tambahAturan(6, hKering, tSedang, pBahaya, sAwas);
    tambahAturan(7, hKering, tTinggi, pAman, sAman);
    tambahAturan(8, hKering, tTinggi, pWaspada, sWaspada);
    tambahAturan(9, hKering, tTinggi, pBahaya, sAwas);

    // hujan gerimis
    tambahAturan(10, hGerimis, tRendah, pAman, sAman);
    tambahAturan(11, hGerimis, tRendah, pWaspada, sWaspada);
    tambahAturan(12, hGerimis, tRendah, pBahaya, sAwas);
    tambahAturan(13, hGerimis, tSedang, pAman, sAman);
    tambahAturan(14, hGerimis, tSedang, pWaspada, sWaspada);
    tambahAturan(15, hGerimis, tSedang, pBahaya, sAwas);
    tambahAturan(16, hGerimis, tTinggi, pAman, sWaspada);
    tambahAturan(17, hGerimis, tTinggi, pWaspada, sAwas);
    tambahAturan(18, hGerimis, tTinggi, pBahaya, sAwas);

    // hujan deras
    tambahAturan(19, hDeras, tRendah, pAman, sAman);
    tambahAturan(20, hDeras, tRendah, pWaspada, sWaspada);
    tambahAturan(21, hDeras, tRendah, pBahaya, sAwas);
    tambahAturan(22, hDeras, tSedang, pAman, sWaspada);
    tambahAturan(23, hDeras, tSedang, pWaspada, sAwas);
    tambahAturan(24, hDeras, tSedang, pBahaya, sAwas);
    tambahAturan(25, hDeras, tTinggi, pAman, sWaspada);
    tambahAturan(26, hDeras, tTinggi, pWaspada, sAwas);
    tambahAturan(27, hDeras, tTinggi, pBahaya, sAwas);
}

void hitungFuzzy(float dataHujan, float dataKelembapan, float dataPergerakan) {
    fuzzy->setInput(1, dataHujan);
    fuzzy->setInput(2, dataKelembapan);
    fuzzy->setInput(3, dataPergerakan);
    fuzzy->fuzzify();

    float hasilPeringatan = fuzzy->defuzzify(1);
    
    Serial.printf("\r\n[FUZZY] Nilai Defuzzifikasi: %.2f\r\n", hasilPeringatan);
    Serial.print("[FUZZY] KEPUTUSAN: ");

    if (hasilPeringatan >= 60) {
        Serial.println("AWAS (Sirine Menyala) 🔴");
    } else if (hasilPeringatan >= 30) {
        Serial.println("WASPADA (Sirine Jeda) 🟡");
    } else {
        Serial.println("AMAN (Sirine Mati) 🟢");
    }
}