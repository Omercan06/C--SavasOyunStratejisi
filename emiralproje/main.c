#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "include/curl/curl.h"

#undef DrawText
#define GRID_SIZE 20
#define CELL_SIZE 30
#define MAX_UNITS_PER_CELL 100
#define CELL_SIZE 45


const char* linkler[] =
{
    "https://yapbenzet.org.tr/1.json",
    "https://yapbenzet.org.tr/2.json",
    "https://yapbenzet.org.tr/3.json",
    "https://yapbenzet.org.tr/4.json",
    "https://yapbenzet.org.tr/5.json",
    "https://yapbenzet.org.tr/6.json",
    "https://yapbenzet.org.tr/7.json",
    "https://yapbenzet.org.tr/8.json",
    "https://yapbenzet.org.tr/9.json",
    "https://yapbenzet.org.tr/10.json"

};

size_t ft_write(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    return fwrite(ptr, size, nmemb, stream);

}

void DosyaIndir(int sayi)
{
    CURL* curl;
    FILE* dosya;
    CURLcode res;

    if (sayi < 1 || sayi >10)
    {
        printf("Gecersiz numara! 1-10 arasinda bir sayi girin.\n");
        return;

    }
    const char *url = linkler[sayi - 1];
    printf("Secilen senaryonun linki: %s\n", url);

    curl = curl_easy_init();
    if (curl)
    {
        dosya = fopen("senaryo.json", "wb");
        if (!dosya)
        {
            perror("dosya acilamadi!");
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ft_write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, dosya);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "Dosya indirilemedi: %s\n", curl_easy_strerror(res));
        }
        fclose(dosya);
        curl_easy_cleanup(curl);

    }

}

 typedef struct
{

    int saldiri;
    int savunma;
    int saglik;
    int kritik_sans;
    int sayi;

} Birlik;

typedef struct
{

    char etki_turu[50];
    int etki_degeri;
    char aciklama[256];
    int sayi;

} Yaratik;

typedef struct
{

    char bonus_turu[50];
    char aciklama[256];
    int bonus_degeri;
    int sayi;

} Kahraman;

typedef struct
{

    Birlik piyadeler;
    Birlik okcular;
    Birlik suvariler;
    Birlik kusatma_makineleri;

} InsanImparatorlugu_Birlik;

typedef struct
{

    Yaratik ejderha;
    Yaratik agri_dagi_devleri;
    Yaratik tepegoz;
    Yaratik karakurt;
    Yaratik samur;

} InsanImparatorlugu_Yaratik;

typedef struct
{
    Kahraman alparslan;
    Kahraman fatih_sultan_mehmet;
    Kahraman yavuz_sultan_selim;
    Kahraman mete_han;
    Kahraman tugrul_bey;

} InsanImparatorlugu_Kahraman;

typedef struct
{

    Birlik ork_dovusculeri;
    Birlik mizrakcilar;
    Birlik varg_binicileri;
    Birlik troller;

} OrkLegi_Birlik;

typedef struct
{

    Yaratik kara_troll;
    Yaratik golge_kurtlari;
    Yaratik camur_devleri;
    Yaratik ates_iblisi;
    Yaratik buz_devleri;
    Yaratik makrog_savas_beyi;

} OrkLegi_Yaratik;

typedef struct
{
    Kahraman goruk_vahsi;
    Kahraman thruk_kemikkiran;
    Kahraman vrog_kafakiran;
    Kahraman ugar_zalim;

} OrkLegi_Kahraman;

typedef struct
{

    int sayi;
    int deger;
    char aciklama[256];

} ArastirmaOzellik;

typedef struct
{

    ArastirmaOzellik seviye_1;
    ArastirmaOzellik seviye_2;
    ArastirmaOzellik seviye_3;

} ArastirmaTuru;

typedef struct
{
    ArastirmaTuru savunma_ustaligi;
    ArastirmaTuru saldiri_gelistirmesi;
    ArastirmaTuru elit_egitim;
    ArastirmaTuru kusatma_ustaligi;

} Arastirma;

InsanImparatorlugu_Birlik insan_birlik;
OrkLegi_Birlik ork_birlik;
InsanImparatorlugu_Kahraman insan_kahraman;
OrkLegi_Kahraman ork_kahraman;
InsanImparatorlugu_Yaratik insan_yaratik;
OrkLegi_Yaratik ork_yaratik;
ArastirmaTuru arastirma_turu;
Arastirma arastirma_insan;
Arastirma arastirma_ork;


void BirlikOzellikOkuma(FILE* dosya, Birlik* birlik)
{
    char satir[256];
    while (fgets(satir, sizeof(satir), dosya))
    {
        if (strstr(satir, "\"saldiri\""))
        {
            sscanf(satir, " \"saldiri\" : %d", &birlik->saldiri);
        }
        else if (strstr(satir, "\"savunma\""))
        {
            sscanf(satir, " \"savunma\" : %d", &birlik->savunma);
        }
        else if (strstr(satir, "\"saglik\""))
        {
            sscanf(satir, " \"saglik\" : %d", &birlik->saglik);
        }
        else if (strstr(satir, "\"kritik_sans\""))
        {
            sscanf(satir, " \"kritik_sans\" : %d", &birlik->kritik_sans);
        }

        if (strstr(satir, "}"))
        {
            break;
        }
    }
}

void jsonBirlikOkuma(const char* dosyayolu, InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik)
{
    char satir[256];
    FILE* dosya = fopen(dosyayolu, "r");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi...\n");
        return;
    }

    Birlik* birlik = NULL;

    while (fgets(satir, sizeof(satir), dosya))
    {
        if (strstr(satir, "\"piyadeler\""))
        {
            birlik = &insan_birlik->piyadeler;
        }
        else if (strstr(satir, "\"okcular\""))
        {
            birlik = &insan_birlik->okcular;
        }
        else if (strstr(satir, "\"suvariler\""))
        {
            birlik = &insan_birlik->suvariler;
        }
        else if (strstr(satir, "\"kusatma_makineleri\""))
        {
            birlik = &insan_birlik->kusatma_makineleri;
        }
        else if (strstr(satir, "\"ork_dovusculeri\""))
        {
            birlik = &ork_birlik->ork_dovusculeri;
        }
        else if (strstr(satir, "\"mizrakcilar\""))
        {
            birlik = &ork_birlik->mizrakcilar;
        }
        else if (strstr(satir, "\"varg_binicileri\""))
        {
            birlik = &ork_birlik->varg_binicileri;
        }
        else if (strstr(satir, "\"troller\""))
        {
            birlik = &ork_birlik->troller;
        }

        if (birlik != NULL)
        {
            BirlikOzellikOkuma(dosya, birlik);
            birlik = NULL;
        }
    }

    fclose(dosya);
}

void jsonBirlikYazdirma(InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik)
{

    printf("Piyadeler\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",insan_birlik->piyadeler.saldiri,
           insan_birlik->piyadeler.savunma,
           insan_birlik->piyadeler.saglik,
           insan_birlik->piyadeler.kritik_sans,
           insan_birlik->piyadeler.sayi);
    printf("Okcular\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",insan_birlik->okcular.saldiri,
           insan_birlik->okcular.savunma,
           insan_birlik->okcular.saglik,
           insan_birlik->okcular.kritik_sans,
           insan_birlik->okcular.sayi);
    printf("Suvariler\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",insan_birlik->suvariler.saldiri,
           insan_birlik->suvariler.savunma,
           insan_birlik->suvariler.saglik,
           insan_birlik->suvariler.kritik_sans,
           insan_birlik->suvariler.sayi);
    printf("Kusatma makineleri\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",insan_birlik->kusatma_makineleri.saldiri,
           insan_birlik->kusatma_makineleri.savunma,
           insan_birlik->kusatma_makineleri.saglik,
           insan_birlik->kusatma_makineleri.kritik_sans,
           insan_birlik->kusatma_makineleri.sayi);
    printf("Ork dovusculeri\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",ork_birlik->ork_dovusculeri.saldiri,
           ork_birlik->ork_dovusculeri.savunma,
           ork_birlik->ork_dovusculeri.saglik,
           ork_birlik->ork_dovusculeri.kritik_sans,
           ork_birlik->ork_dovusculeri.sayi);
    printf("Mizrakcilar\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",ork_birlik->mizrakcilar.saldiri,
           ork_birlik->mizrakcilar.savunma,
           ork_birlik->mizrakcilar.saglik,
           ork_birlik->mizrakcilar.kritik_sans,
           ork_birlik->mizrakcilar.sayi);
    printf("Varg binicileri\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",ork_birlik->varg_binicileri.saldiri,
           ork_birlik->varg_binicileri.savunma,
           ork_birlik->varg_binicileri.saglik,
           ork_birlik->varg_binicileri.kritik_sans,
           ork_birlik->varg_binicileri.sayi);
    printf("Troller\n saldiri:%d\nsavunma:%d\nsaglik:%d\nkritik sans:%d\nsayi:%d\n",ork_birlik->troller.saldiri,
           ork_birlik->troller.savunma,
           ork_birlik->troller.saglik,
           ork_birlik->troller.kritik_sans,
           ork_birlik->troller.sayi);

}
void KahramanOzellikOkuma(FILE* dosya,Kahraman* kahraman)
{
    char satir[256];
    char bonus_degeri_str[10];
    while (fgets(satir, sizeof(satir), dosya))
    {
        if (strstr(satir, "\"bonus_turu\""))
        {
            sscanf(satir, " \"bonus_turu\" : \"%[^\"]\"", kahraman->bonus_turu);
        }

        else if (strstr(satir, "\"bonus_degeri\""))
        {
            sscanf(satir, " \"bonus_degeri\" : \"%[^\"]\"", bonus_degeri_str);
            kahraman->bonus_degeri=atoi(bonus_degeri_str);

        }
        else if (strstr(satir, "\"aciklama\""))
        {
            sscanf(satir, " \"aciklama\" : \"%[^\"]\"", kahraman->aciklama);
        }

        if (strstr(satir, "}"))
        {
            break;
        }
    }

}

void jsonKahramanOkuma(const char* dosyayolu,InsanImparatorlugu_Kahraman *insan_kahraman,OrkLegi_Kahraman *ork_kahraman)
{

    FILE* dosya=fopen(dosyayolu,"r");
    if(dosya==NULL)
    {
        printf("Dosya acilamadi...");
        return;
    }
    char satir[256];
    Kahraman* kahraman=NULL;

    while (fgets(satir, sizeof(satir), dosya))
    {
        if (strstr(satir, "\"Alparslan\""))
        {
            kahraman = &insan_kahraman->alparslan;
        }
        else if (strstr(satir, "\"Fatih_Sultan_Mehmet\""))
        {
            kahraman = &insan_kahraman->fatih_sultan_mehmet;
        }
        else if (strstr(satir, "\"Yavuz_Sultan_Selim\""))
        {
            kahraman = &insan_kahraman->yavuz_sultan_selim;
        }
        else if (strstr(satir, "\"Mete_Han\""))
        {
            kahraman = &insan_kahraman->mete_han;
        }
        else if (strstr(satir, "\"Tugrul_Bey\""))
        {
            kahraman = &insan_kahraman->tugrul_bey;
        }
        else if (strstr(satir, "\"Goruk_Vahsi\""))
        {
            kahraman = &ork_kahraman->goruk_vahsi;
        }
        else if (strstr(satir, "\"Thruk_Kemikkiran\""))
        {
            kahraman = &ork_kahraman->thruk_kemikkiran;
        }
        else if (strstr(satir, "\"Vrog_Kafakiran\""))
        {
            kahraman = &ork_kahraman->vrog_kafakiran;
        }
        else if (strstr(satir, "\"Ugar_Zalim\""))
        {
            kahraman = &ork_kahraman->ugar_zalim;
        }

        if (kahraman != NULL)
        {
            KahramanOzellikOkuma(dosya, kahraman);
            kahraman = NULL;
        }
    }

    fclose(dosya);

}

void jsonKahramanYazdirma(InsanImparatorlugu_Kahraman *insan_kahraman,OrkLegi_Kahraman *ork_kahraman)
{

    printf("\n\n--------------------------------------------------\n\n");
    printf("Alparslan\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",insan_kahraman->alparslan.bonus_turu,
           insan_kahraman->alparslan.bonus_degeri,
           insan_kahraman->alparslan.aciklama);
    printf("FSM\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",insan_kahraman->fatih_sultan_mehmet.bonus_turu,
           insan_kahraman->fatih_sultan_mehmet.bonus_degeri,
           insan_kahraman->fatih_sultan_mehmet.aciklama);
    printf("Mete Han\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",insan_kahraman->mete_han.bonus_turu,
           insan_kahraman->mete_han.bonus_degeri,
           insan_kahraman->mete_han.aciklama);
    printf("YSS\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",insan_kahraman->yavuz_sultan_selim.bonus_turu,
           insan_kahraman->yavuz_sultan_selim.bonus_degeri,
           insan_kahraman->yavuz_sultan_selim.aciklama);
    printf("Tugrul Bey\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",insan_kahraman->tugrul_bey.bonus_turu,
           insan_kahraman->tugrul_bey.bonus_degeri,
           insan_kahraman->tugrul_bey.aciklama);
    printf("Goruk Vahsi\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",ork_kahraman->goruk_vahsi.bonus_turu,
           ork_kahraman->goruk_vahsi.bonus_degeri,
           ork_kahraman->goruk_vahsi.aciklama);
    printf("TK\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",ork_kahraman->thruk_kemikkiran.bonus_turu,
           ork_kahraman->thruk_kemikkiran.bonus_degeri,
           ork_kahraman->thruk_kemikkiran.aciklama);
    printf("VK\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",ork_kahraman->vrog_kafakiran.bonus_turu,
           ork_kahraman->vrog_kafakiran.bonus_degeri,
           ork_kahraman->vrog_kafakiran.aciklama);
    printf("UZ\nbonus turu:%s\nbonus degeri:%d\naciklama:%s\n",ork_kahraman->ugar_zalim.bonus_turu,
           ork_kahraman->ugar_zalim.bonus_degeri,
           ork_kahraman->ugar_zalim.aciklama);

}

void YaratikOzellikOkuma(FILE* dosya,Yaratik* yaratik)
{
    char satir[256];
    char etki_degeri_str[10];

    while(fgets(satir,sizeof(satir),dosya))
    {

        if(strstr(satir, "\"etki_degeri\""))
        {
            sscanf(satir, " \"etki_degeri\" : \"%[^\"]\"", etki_degeri_str);
            yaratik->etki_degeri=atoi(etki_degeri_str);
        }
        else if(strstr(satir, "\"etki_turu\""))
        {
            sscanf(satir, " \"etki_turu\" : \"%[^\"]\"", yaratik->etki_turu);

        }
        else if (strstr(satir, "\"aciklama\""))
        {
            sscanf(satir, " \"aciklama\" : \"%[^\"]\"", yaratik->aciklama);
        }


        if(strstr(satir,"}"))
        {
            return;
        }

    }


}

void jsonYaratikOkuma(const char* dosyayolu,InsanImparatorlugu_Yaratik *insan_yaratik,OrkLegi_Yaratik *ork_yaratik)
{

    FILE* dosya=fopen(dosyayolu,"r");
    if(dosya==NULL)
    {
        printf("Dosya acilamadi...");
        return;
    }

    char satir[256];
    Yaratik* yaratik=NULL;


    while(fgets(satir,sizeof(satir),dosya))
    {

        if(strstr(satir, "\"Ejderha\""))
        {
            yaratik=&insan_yaratik->ejderha;

        }
        else if(strstr(satir, "\"Agri_Dagi_Devleri\""))
        {
            yaratik=&insan_yaratik->agri_dagi_devleri;

        }
        else if(strstr(satir, "\"Tepegoz\""))
        {
            yaratik=&insan_yaratik->tepegoz;

        }
        else if(strstr(satir, "\"Karakurt\""))
        {
            yaratik=&insan_yaratik->karakurt;

        }
        else if(strstr(satir, "\"Samur\""))
        {
            yaratik=&insan_yaratik->samur;

        }
        else if(strstr(satir, "\"Kara_Troll\""))
        {
            yaratik=&ork_yaratik->kara_troll;

        }
        else if(strstr(satir, "\"Golge_Kurtlari\""))
        {
            yaratik=&ork_yaratik->golge_kurtlari;

        }
        else if(strstr(satir, "\"Camur_Devleri\""))
        {
            yaratik=&ork_yaratik->camur_devleri;

        }
        else if(strstr(satir, "\"Ates_Iblisi\""))
        {
            yaratik=&ork_yaratik->ates_iblisi;

        }
        else if (strstr(satir, "\"Makrog_Savas_Beyi\""))
        {
            yaratik= &ork_yaratik->makrog_savas_beyi;

        }
        else if(strstr(satir, "\"Buz_Devleri\""))
        {
            yaratik=&ork_yaratik->buz_devleri;
        }

        if (yaratik != NULL)
        {
            YaratikOzellikOkuma(dosya, yaratik);
            yaratik = NULL;
        }

    }

    fclose(dosya);


}

void jsonYaratikYazdirma(InsanImparatorlugu_Yaratik *insan_yaratik,OrkLegi_Yaratik *ork_yaratik)
{

    printf("\n------------------------------------------\n\n");
    printf("Ejderha\netki degeri:%d\netki turu:%s\naciklama:%s\n",insan_yaratik->ejderha.etki_degeri,
           insan_yaratik->ejderha.etki_turu,
           insan_yaratik->ejderha.aciklama);
    printf("ADD\netki degeri:%d\netki turu:%s\naciklama:%s\n",insan_yaratik->agri_dagi_devleri.etki_degeri,
           insan_yaratik->agri_dagi_devleri.etki_turu,
           insan_yaratik->agri_dagi_devleri.aciklama);
    printf("TEpegoz\netki degeri:%d\netki turu:%s\naciklama:%s\n",insan_yaratik->tepegoz.etki_degeri,
           insan_yaratik->tepegoz.etki_turu,
           insan_yaratik->tepegoz.aciklama);
    printf("Karakurt\netki degeri:%d\netki turu:%s\naciklama:%s\n",insan_yaratik->karakurt.etki_degeri,
           insan_yaratik->karakurt.etki_turu,
           insan_yaratik->karakurt.aciklama);
    printf("Samur\netki degeri:%d\netki turu:%s\naciklama:%s\n",insan_yaratik->samur.etki_degeri,
           insan_yaratik->samur.etki_turu,
           insan_yaratik->samur.aciklama);
    printf("KaraTroll\netki degeri:%d\netki turu:%s\naciklama:%s\n",ork_yaratik->kara_troll.etki_degeri,
           ork_yaratik->kara_troll.etki_turu,
           ork_yaratik->kara_troll.aciklama);
    printf("GK\netki degeri:%d\netki turu:%s\naciklama:%s\n",ork_yaratik->golge_kurtlari.etki_degeri,
           ork_yaratik->golge_kurtlari.etki_turu,
           ork_yaratik->golge_kurtlari.aciklama);
    printf("CD\netki degeri:%d\netki turu:%s\naciklama:%s",ork_yaratik->camur_devleri.etki_degeri,
           ork_yaratik->camur_devleri.etki_turu,
           ork_yaratik->camur_devleri.aciklama);
    printf("AI\netki degeri:%d\netki turu:%s\naciklama:%s\n",ork_yaratik->ates_iblisi.etki_degeri,
           ork_yaratik->ates_iblisi.etki_turu,
           ork_yaratik->ates_iblisi.aciklama);
    printf("MSB\netki degeri:%d\netki turu:%s\naciklama:%s\n",ork_yaratik->makrog_savas_beyi.etki_degeri,
           ork_yaratik->makrog_savas_beyi.etki_turu,
           ork_yaratik->makrog_savas_beyi.aciklama);
    printf("BD\netki degeri:%d\netki turu:%s\naciklama:%s\n",ork_yaratik->buz_devleri.etki_degeri,
           ork_yaratik->buz_devleri.etki_turu,
           ork_yaratik->buz_devleri.aciklama);

}

void jsonArastirmaOkuma(const char* dosyayolu,Arastirma *arastirma_insan,Arastirma *arastirma_ork)
{
    FILE *dosya=fopen(dosyayolu,"r");
    if(dosya==NULL)
    {
        printf("Dosya aacilamadi..");
        return;
    }
    char satir[256];
    ArastirmaTuru *arastirma_tipi=NULL;
    ArastirmaOzellik *arastirma_ozellik=NULL;

    while(fgets(satir,sizeof(satir),dosya))
    {

        if(strstr(satir, "\"savunma_ustaligi\""))
        {

            arastirma_tipi=&arastirma_insan->savunma_ustaligi;
        }
        else if(strstr(satir, "\"saldiri_gelistirmesi\""))
        {

            arastirma_tipi=&arastirma_insan->saldiri_gelistirmesi;
        }
        else if(strstr(satir, "\"elit_egitim\""))
        {

            arastirma_tipi=&arastirma_insan->elit_egitim;
        }
        else if(strstr(satir, "\"kusatma_ustaligi\""))
        {

            arastirma_tipi=&arastirma_insan->kusatma_ustaligi;
        }
        else if(strstr(satir, "\"deger\""))
        {

            if(arastirma_ozellik)
            {

                sscanf(satir, " \"deger\": \"%d\"", &arastirma_ozellik->deger);
            }
        }
        else if(strstr(satir, "\"aciklama\""))
        {

            if(arastirma_ozellik)
            {

                char *start = strchr(satir, ':') + 3;
                strncpy(arastirma_ozellik->aciklama, start, strlen(start) - 2);
                arastirma_ozellik->aciklama[strlen(start) - 2] = '\0';
            }
        }
        else if(strstr(satir, "\"seviye_1\""))
        {

            arastirma_ozellik=&arastirma_tipi->seviye_1;
        }
        else if(strstr(satir, "\"seviye_2\""))
        {

            arastirma_ozellik=&arastirma_tipi->seviye_2;
        }
        else if(strstr(satir, "\"seviye_3\""))
        {

            arastirma_ozellik=&arastirma_tipi->seviye_3;
        }

    }
    fclose(dosya);

    memcpy(&arastirma_ork->savunma_ustaligi, &arastirma_insan->savunma_ustaligi, sizeof(ArastirmaTuru));
    memcpy(&arastirma_ork->saldiri_gelistirmesi, &arastirma_insan->saldiri_gelistirmesi, sizeof(ArastirmaTuru));
    memcpy(&arastirma_ork->elit_egitim, &arastirma_insan->elit_egitim, sizeof(ArastirmaTuru));
    memcpy(&arastirma_ork->kusatma_ustaligi, &arastirma_insan->kusatma_ustaligi, sizeof(ArastirmaTuru));


}
void yazdir_seviye(ArastirmaOzellik *arastirma_ozellik)
{
    printf("Deðer: %d, Açýklama: %s, Sayý: %d\n", arastirma_ozellik->deger, arastirma_ozellik->aciklama, arastirma_ozellik->sayi);
}

void yazdir_human_research(Arastirma *arastirma_insan)
{
    printf("Insan Arastirma: \n");
    printf("Savunma Ustalýðý Seviye 1:\n");
    yazdir_seviye(&arastirma_insan->savunma_ustaligi.seviye_1);
    printf("Savunma Ustalýðý Seviye 2:\n");
    yazdir_seviye(&arastirma_insan->savunma_ustaligi.seviye_2);
    printf("Savunma Ustalýðý Seviye 3:\n");
    yazdir_seviye(&arastirma_insan->savunma_ustaligi.seviye_3);

    printf("Saldýrý Geliþtirmesi Seviye 1:\n");
    yazdir_seviye(&arastirma_insan->saldiri_gelistirmesi.seviye_1);
    printf("Saldýrý Geliþtirmesi Seviye 2:\n");
    yazdir_seviye(&arastirma_insan->saldiri_gelistirmesi.seviye_2);
    printf("Saldýrý Geliþtirmesi Seviye 3:\n");
    yazdir_seviye(&arastirma_insan->saldiri_gelistirmesi.seviye_3);

    printf("Elit Eðitim Seviye 1:\n");
    yazdir_seviye(&arastirma_insan->elit_egitim.seviye_1);
    printf("Elit Eðitim Seviye 2:\n");
    yazdir_seviye(&arastirma_insan->elit_egitim.seviye_2);
    printf("Elit Eðitim Seviye 3:\n");
    yazdir_seviye(&arastirma_insan->elit_egitim.seviye_3);

    printf("Kuþatma Ustalýðý Seviye 1:\n");
    yazdir_seviye(&arastirma_insan->kusatma_ustaligi.seviye_1);
    printf("Kuþatma Ustalýðý Seviye 2:\n");
    yazdir_seviye(&arastirma_insan->kusatma_ustaligi.seviye_2);
    printf("Kuþatma Ustalýðý Seviye 3:\n");
    yazdir_seviye(&arastirma_insan->kusatma_ustaligi.seviye_3);
}

void yazdir_ork_research(Arastirma *arastirma_ork)
{
    printf("Ork Arastirma: \n");
    printf("Savunma Ustalýðý Seviye 1:\n");
    yazdir_seviye(&arastirma_ork->savunma_ustaligi.seviye_1);
    printf("Savunma Ustalýðý Seviye 2:\n");
    yazdir_seviye(&arastirma_ork->savunma_ustaligi.seviye_2);
    printf("Savunma Ustalýðý Seviye 3:\n");
    yazdir_seviye(&arastirma_ork->savunma_ustaligi.seviye_3);

    printf("Saldýrý Geliþtirmesi Seviye 1:\n");
    yazdir_seviye(&arastirma_ork->saldiri_gelistirmesi.seviye_1);
    printf("Saldýrý Geliþtirmesi Seviye 2:\n");
    yazdir_seviye(&arastirma_ork->saldiri_gelistirmesi.seviye_2);
    printf("Saldýrý Geliþtirmesi Seviye 3:\n");
    yazdir_seviye(&arastirma_ork->saldiri_gelistirmesi.seviye_3);

    printf("Elit Eðitim Seviye 1:\n");
    yazdir_seviye(&arastirma_ork->elit_egitim.seviye_1);
    printf("Elit Eðitim Seviye 2:\n");
    yazdir_seviye(&arastirma_ork->elit_egitim.seviye_2);
    printf("Elit Eðitim Seviye 3:\n");
    yazdir_seviye(&arastirma_ork->elit_egitim.seviye_3);

    printf("Kuþatma Ustalýðý Seviye 1:\n");
    yazdir_seviye(&arastirma_ork->kusatma_ustaligi.seviye_1);
    printf("Kuþatma Ustalýðý Seviye 2:\n");
    yazdir_seviye(&arastirma_ork->kusatma_ustaligi.seviye_2);
    printf("Kuþatma Ustalýðý Seviye 3:\n");
    yazdir_seviye(&arastirma_ork->kusatma_ustaligi.seviye_3);
}

void jsonInsanSenaryosu(const char* dosyayolu,InsanImparatorlugu_Birlik *insan_birlik,InsanImparatorlugu_Kahraman *insan_kahraman,InsanImparatorlugu_Yaratik *insan_yaratik,Arastirma *arastirma_insan)
{

    FILE *dosya=fopen(dosyayolu,"r");
    if(dosya==NULL)
    {
        printf("Dosya acilamadi..");
        return;
    }
    char satir[256];
    int gecici=0;

    while(fgets(satir, sizeof(satir),dosya))
    {
        if(strstr(satir, "\"insan_imparatorlugu\": {")!=NULL)
        {

            while(fgets(satir, sizeof(satir), dosya) && !strstr(satir, "ork_legi"))
            {

                if(strstr(satir, "\"piyadeler\"")!=NULL)
                {
                    sscanf(satir, " \"piyadeler\": %d,",&insan_birlik->piyadeler.sayi);
                }
                if(strstr(satir, "\"okcular\"")!=NULL)
                {
                    sscanf(satir, " \"okcular\": %d,",&insan_birlik->okcular.sayi);
                }
                if(strstr(satir, "\"suvariler\"")!=NULL)
                {
                    sscanf(satir, " \"suvariler\": %d,",&insan_birlik->suvariler.sayi);
                }
                if(strstr(satir, "\"kusatma_makineleri\"")!=NULL)
                {
                    sscanf(satir, " \"kusatma_makineleri\": %d,",&insan_birlik->kusatma_makineleri.sayi);
                }

                insan_kahraman->alparslan.sayi = strstr(satir, "\"alparslan\"") != NULL ? 1 : 0;
                insan_kahraman->fatih_sultan_mehmet.sayi = strstr(satir, "\"fatih_sultan_mehmet\"") != NULL ? 1 : 0;
                insan_kahraman->yavuz_sultan_selim.sayi = strstr(satir, "\"yavuz_sultan_selim\"") != NULL ? 1 : 0;
                insan_kahraman->mete_han.sayi = strstr(satir, "\"mete_han\"") != NULL ? 1 : 0;
                insan_kahraman->tugrul_bey.sayi = strstr(satir, "\"tugrul_bey\"") != NULL ? 1 : 0;


                insan_yaratik->ejderha.sayi = strstr(satir, "\"ejderha\"") != NULL ? 1 : 0;
                insan_yaratik->agri_dagi_devleri.sayi = strstr(satir, "\"agri_dagi_devleri\"") != NULL ? 1 : 0;
                insan_yaratik->tepegoz.sayi = strstr(satir, "\"tepegoz\"") != NULL ? 1 : 0;
                insan_yaratik->karakurt.sayi = strstr(satir, "\"karakurt\"") != NULL ? 1 : 0;
                insan_yaratik->samur.sayi = strstr(satir, "\"samur\"") != NULL ? 1 : 0;


                if(strstr(satir, "\"savunma_ustaligi\"")!=NULL)
                {

                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_insan->savunma_ustaligi.seviye_1.sayi = 1;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_insan->savunma_ustaligi.seviye_2.sayi = 1;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_insan->savunma_ustaligi.seviye_3.sayi = 1;
                    }

                }

                if (strstr(satir, "\"saldiri_gelistirmesi\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_insan->saldiri_gelistirmesi.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_insan->saldiri_gelistirmesi.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_insan->saldiri_gelistirmesi.seviye_3.sayi++;
                    }
                }
                if (strstr(satir, "\"elit_egitim\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_insan->elit_egitim.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_insan->elit_egitim.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_insan->elit_egitim.seviye_3.sayi++;
                    }
                }
                if (strstr(satir, "\"kusatma_ustaligi\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_insan->kusatma_ustaligi.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_insan->kusatma_ustaligi.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_insan->kusatma_ustaligi.seviye_3.sayi++;
                    }
                }

            }

        }


    }
    fclose(dosya);

}

void jsonOrkSenaryosu(const char* dosyayolu,OrkLegi_Birlik *ork_birlik,OrkLegi_Kahraman *ork_kahraman,OrkLegi_Yaratik *ork_yaratik,Arastirma *arastirma_ork)
{
    FILE *dosya=fopen(dosyayolu,"r");
    if(dosya==NULL)
    {
        printf("Dosya acilamadi..");
        return;
    }
    char satir[256];
    int gecici=0;

    while(fgets(satir,sizeof(satir),dosya))
    {
        if (strstr(satir, "\"ork_legi\": {") != NULL)
        {
            while (fgets(satir, sizeof(satir), dosya) && !strstr(satir, "insan_imparatorlugu"))
            {

                if (strstr(satir, "ork_dovusculeri") != NULL)
                {
                    sscanf(satir, " \"ork_dovusculeri\": %d,", &ork_birlik->ork_dovusculeri.sayi);
                }
                if (strstr(satir, "mizrakcilar") != NULL)
                {
                    sscanf(satir, " \"mizrakcilar\": %d,", &ork_birlik->mizrakcilar.sayi);
                }
                if (strstr(satir, "varg_binicileri") != NULL)
                {
                    sscanf(satir, " \"varg_binicileri\": %d,", &ork_birlik->varg_binicileri.sayi);
                }
                if (strstr(satir, "troller") != NULL)
                {
                    sscanf(satir, " \"troller\": %d,", &ork_birlik->troller.sayi);
                }


                ork_kahraman->goruk_vahsi.sayi = strstr(satir, "\"goruk_vahsi\"") != NULL ? 1 : 0;
                ork_kahraman->thruk_kemikkiran.sayi = strstr(satir, "\"thruk_kemikkiran\"") != NULL ? 1 : 0;
                ork_kahraman->vrog_kafakiran.sayi = strstr(satir, "\"vrog_kafakiran\"") != NULL ? 1 : 0;
                ork_kahraman->ugar_zalim.sayi = strstr(satir, "\"ugar_zalim\"") != NULL ? 1 : 0;

                ork_yaratik->kara_troll.sayi = strstr(satir, "\"kara_troll\"") != NULL ? 1 : 0;
                ork_yaratik->golge_kurtlari.sayi = strstr(satir, "\"golge_kurtlari\"") != NULL ? 1 : 0;
                ork_yaratik->camur_devleri.sayi = strstr(satir, "\"camur_devleri\"") != NULL ? 1 : 0;
                ork_yaratik->ates_iblisi.sayi = strstr(satir, "\"ates_iblisi\"") != NULL ? 1 : 0;
                ork_yaratik->buz_devleri.sayi = strstr(satir, "\"buz_devleri\"") != NULL ? 1 : 0;
                ork_yaratik->makrog_savas_beyi.sayi=strstr(satir, "\"makrog_savas_beyi\"") !=NULL ? 1 : 0;


                if (strstr(satir, "\"savunma_ustaligi\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_ork->savunma_ustaligi.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_ork->savunma_ustaligi.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_ork->savunma_ustaligi.seviye_3.sayi++;
                    }
                }

                if (strstr(satir, "\"saldiri_gelistirmesi\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_ork->saldiri_gelistirmesi.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_ork->saldiri_gelistirmesi.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_ork->saldiri_gelistirmesi.seviye_3.sayi++;
                    }
                }
                if (strstr(satir, "\"elit_egitim\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_ork->elit_egitim.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_ork->elit_egitim.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_ork->elit_egitim.seviye_3.sayi++;
                    }
                }
                if (strstr(satir, "\"kusatma_ustaligi\"") != NULL)
                {
                    sscanf(satir, " \"%*[^\"]\": %d", &gecici);
                    if (gecici == 1)
                    {
                        arastirma_ork->kusatma_ustaligi.seviye_1.sayi++;
                    }
                    else if (gecici == 2)
                    {
                        arastirma_ork->kusatma_ustaligi.seviye_2.sayi++;
                    }
                    else if (gecici == 3)
                    {
                        arastirma_ork->kusatma_ustaligi.seviye_3.sayi++;
                    }
                }


            }

        }

    }

    fclose(dosya);

}
int KritikVurusHesaplama(Birlik *birlik,int devir)
{
    if(birlik->kritik_sans==0)
    {
        return birlik->saldiri * birlik->sayi;
    }
    if(devir % (100 / birlik->kritik_sans)==0)
    {
        return birlik->saldiri * birlik->sayi * 1.5;
    }
    return birlik->saldiri * birlik->sayi;
}

void SaldiriGucuHesaplama(InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik,int devir,double *insan_toplam_saldiri,double *ork_toplam_saldiri)
{
    *insan_toplam_saldiri=KritikVurusHesaplama(&insan_birlik->piyadeler,devir)+
                          KritikVurusHesaplama(&insan_birlik->okcular,devir)+
                          KritikVurusHesaplama(&insan_birlik->suvariler,devir)+
                          KritikVurusHesaplama(&insan_birlik->kusatma_makineleri,devir);

    *ork_toplam_saldiri=KritikVurusHesaplama(&ork_birlik->ork_dovusculeri,devir)+
                        KritikVurusHesaplama(&ork_birlik->mizrakcilar,devir)+
                        KritikVurusHesaplama(&ork_birlik->troller,devir)+
                        KritikVurusHesaplama(&ork_birlik->varg_binicileri,devir);
}
void SavunmaGucuHesaplama(InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik,double *insan_toplam_savunma,double *ork_toplam_savunma)
{
    *insan_toplam_savunma=(insan_birlik->piyadeler.savunma * insan_birlik->piyadeler.sayi)+
                          (insan_birlik->okcular.savunma * insan_birlik->okcular.sayi)+
                          (insan_birlik->suvariler.savunma * insan_birlik->suvariler.sayi)+
                          (insan_birlik->kusatma_makineleri.savunma * insan_birlik->kusatma_makineleri.sayi);

    *ork_toplam_savunma=(ork_birlik->ork_dovusculeri.savunma * ork_birlik->ork_dovusculeri.sayi)+
                        (ork_birlik->mizrakcilar.savunma * ork_birlik->mizrakcilar.sayi)+
                        (ork_birlik->troller.savunma * ork_birlik->troller.sayi)+
                        (ork_birlik->varg_binicileri.savunma * ork_birlik->varg_binicileri.sayi);
}
void InsanSaglikHesaplama(InsanImparatorlugu_Birlik *insan_birlik,double ork_net_hasar,double *insan_toplam_savunma)
{

    double piyadeler_savunma_orani = (double)(insan_birlik->piyadeler.savunma * insan_birlik->piyadeler.sayi)/(*insan_toplam_savunma);
    double okcular_savunma_orani=(double)(insan_birlik->okcular.savunma*insan_birlik->okcular.sayi)/(*insan_toplam_savunma);
    double suvariler_savunma_orani=(double)(insan_birlik->suvariler.savunma*insan_birlik->suvariler.sayi)/(*insan_toplam_savunma);
    double kusatmaMak_savunma_orani = (double)(insan_birlik->kusatma_makineleri.savunma*insan_birlik->kusatma_makineleri.sayi)/(*insan_toplam_savunma);



    double piyadeler_alinan_hasar=(piyadeler_savunma_orani)*(ork_net_hasar);
    double okcular_alinan_hasar=(okcular_savunma_orani)*(ork_net_hasar);
    double suvariler_alinan_hasar=(suvariler_savunma_orani)*(ork_net_hasar);
    double kusatmaMak_alinan_hasar=(kusatmaMak_savunma_orani)*(ork_net_hasar);


    insan_birlik->piyadeler.saglik -= piyadeler_alinan_hasar / insan_birlik->piyadeler.sayi;
    insan_birlik->okcular.saglik -= okcular_alinan_hasar / insan_birlik->piyadeler.sayi;
    insan_birlik->suvariler.saglik -= suvariler_alinan_hasar / insan_birlik->suvariler.sayi;
    insan_birlik->kusatma_makineleri.saglik -= kusatmaMak_alinan_hasar / insan_birlik->kusatma_makineleri.sayi;


    if (insan_birlik->piyadeler.saglik <= 0) insan_birlik->piyadeler.saglik = 0;
    if (insan_birlik->okcular.saglik <= 0) insan_birlik->okcular.saglik = 0;
    if (insan_birlik->suvariler.saglik <= 0) insan_birlik->suvariler.saglik = 0;
    if (insan_birlik->kusatma_makineleri.saglik <= 0) insan_birlik->kusatma_makineleri.saglik = 0;


    if (insan_birlik->piyadeler.saglik <= 0) insan_birlik->piyadeler.sayi = 0;
    if (insan_birlik->okcular.saglik <= 0) insan_birlik->okcular.sayi = 0;
    if (insan_birlik->suvariler.saglik <= 0) insan_birlik->suvariler.sayi = 0;
    if (insan_birlik->kusatma_makineleri.saglik <= 0) insan_birlik->kusatma_makineleri.sayi = 0;

}

void OrkSaglikHesaplama(OrkLegi_Birlik *ork_birlik,double insan_net_hasar,double *ork_toplam_savunma)
{
    double ork_dovusculeri_savunma_orani = (double)(ork_birlik->ork_dovusculeri.savunma * ork_birlik->ork_dovusculeri.sayi)/(*ork_toplam_savunma);
    double mizrakcilar_savunma_orani=(double)(ork_birlik->mizrakcilar.savunma * ork_birlik->mizrakcilar.sayi)/(*ork_toplam_savunma);
    double troller_savunma_orani=(double)(ork_birlik->troller.savunma * ork_birlik->troller.sayi)/(*ork_toplam_savunma);
    double varg_binicileri_savunma_orani = (double)(ork_birlik->varg_binicileri.savunma * ork_birlik->varg_binicileri.sayi)/(*ork_toplam_savunma);


    double ork_dovusculeri_alinan_hasar=(ork_dovusculeri_savunma_orani)*(insan_net_hasar);
    double mizrakcilar_alinan_hasar=(mizrakcilar_savunma_orani)*(insan_net_hasar);
    double troller_alinan_hasar=(troller_savunma_orani)*(insan_net_hasar);
    double varg_binicileri_alinan_hasar=(varg_binicileri_savunma_orani)*(insan_net_hasar);

    ork_birlik->ork_dovusculeri.saglik -= (ork_dovusculeri_alinan_hasar / ork_birlik->ork_dovusculeri.sayi);
    ork_birlik->mizrakcilar.saglik -= (ork_dovusculeri_alinan_hasar / ork_birlik->mizrakcilar.sayi);
    ork_birlik->troller.saglik -= (ork_dovusculeri_alinan_hasar / ork_birlik->troller.sayi);
    ork_birlik->varg_binicileri.saglik -= (ork_dovusculeri_alinan_hasar / ork_birlik->varg_binicileri.sayi);


    if (ork_birlik->ork_dovusculeri.saglik <= 0) ork_birlik->ork_dovusculeri.saglik = 0;
    if (ork_birlik->mizrakcilar.saglik <= 0) ork_birlik->mizrakcilar.saglik = 0;
    if (ork_birlik->troller.saglik <= 0) ork_birlik->troller.saglik = 0;
    if (ork_birlik->varg_binicileri.saglik <= 0) ork_birlik->varg_binicileri.saglik = 0;


    if (ork_birlik->ork_dovusculeri.saglik <= 0) ork_birlik->ork_dovusculeri.sayi = 0;
    if (ork_birlik->mizrakcilar.saglik <= 0) ork_birlik->mizrakcilar.sayi = 0;
    if (ork_birlik->troller.saglik <= 0) ork_birlik->troller.sayi = 0;
    if (ork_birlik->varg_binicileri.saglik <= 0) ork_birlik->varg_binicileri.sayi = 0;

}



int insanNetHasar(InsanImparatorlugu_Birlik *insan_birlik,double *insan_toplam_saldiri,double *ork_toplam_savunma)
{
    int insan_net_hasar = *insan_toplam_saldiri * (1 - (*ork_toplam_savunma / *insan_toplam_saldiri));
    if (insan_net_hasar < 0)
        insan_net_hasar = 0;
    return (insan_net_hasar);
}

int orkNetHasar(OrkLegi_Birlik *ork_birlik,double *ork_toplam_saldiri,double *insan_toplam_savunma)
{
    int ork_net_hasar = *ork_toplam_saldiri * (1 - (*insan_toplam_savunma / *ork_toplam_saldiri));
    if (ork_net_hasar < 0)
        ork_net_hasar = 0;
    return (ork_net_hasar);
}
void BonusDeger(InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik,InsanImparatorlugu_Kahraman *insan_kahraman,OrkLegi_Kahraman *ork_kahraman,InsanImparatorlugu_Yaratik *insan_yaratik,OrkLegi_Yaratik *ork_yaratik,Arastirma *arastirma_insan,Arastirma *arastirma_ork)
{

    if(insan_kahraman->alparslan.sayi==1)
    {
        insan_birlik->piyadeler.savunma *= 1.2;
    }

    if(insan_kahraman->fatih_sultan_mehmet.sayi==1)
    {
        insan_birlik->kusatma_makineleri.saldiri *= 1.25;
    }

    if(insan_kahraman->mete_han.sayi==1)
    {
        insan_birlik->okcular.savunma *= 1.2;
    }

    if(insan_kahraman->yavuz_sultan_selim.sayi==1)
    {
        insan_birlik->suvariler.kritik_sans += 15;
    }

    if(insan_kahraman->tugrul_bey.sayi==1)
    {
        insan_birlik->okcular.saldiri *= 1.2;
    }

    if(ork_kahraman->goruk_vahsi.sayi==1)
    {
        ork_birlik->ork_dovusculeri.saldiri *= 1.2;
    }

    if(ork_kahraman->thruk_kemikkiran.sayi==1)
    {
        ork_birlik->troller.savunma*=1.25;
    }

    if(ork_kahraman->vrog_kafakiran.sayi==1)
    {
        ork_birlik->varg_binicileri.kritik_sans += 15;
    }

    if(ork_kahraman->ugar_zalim.sayi==1)
    {
        ork_birlik->mizrakcilar.savunma *=1.1;
        ork_birlik->ork_dovusculeri.savunma *=1.1;
        ork_birlik->troller.savunma *=1.1;
        ork_birlik->varg_binicileri.savunma *=1.1;
    }

    if(insan_yaratik->ejderha.sayi==1)
    {
        insan_birlik->piyadeler.saldiri *=1.15;
    }

    if(insan_yaratik->agri_dagi_devleri.sayi==1)
    {
        insan_birlik->suvariler.savunma *=1.2;
    }

    if(insan_yaratik->tepegoz.sayi==1)
    {
        insan_birlik->okcular.saldiri *=1.25;
    }

    if(insan_yaratik->karakurt.sayi==1)
    {
        insan_birlik->okcular.kritik_sans += 10;
    }

    if(insan_yaratik->samur.sayi==1)
    {
        insan_birlik->piyadeler.savunma *=1.1;
    }

    if(ork_yaratik->kara_troll.sayi==1)
    {
        ork_birlik->troller.saldiri *=1.2;
    }

    if(ork_yaratik->golge_kurtlari.sayi==1)
    {
        ork_birlik->varg_binicileri.savunma *=1.15;
    }

    if(ork_yaratik->camur_devleri.sayi==1)
    {
        ork_birlik->ork_dovusculeri.savunma *=1.25;
    }

    if(ork_yaratik->ates_iblisi.sayi==1)
    {
        ork_birlik->varg_binicileri.saldiri *=1.3;
    }

    if(ork_yaratik->makrog_savas_beyi.sayi==1)
    {
        ork_birlik->troller.savunma *=1.25;
    }

    if(ork_yaratik->buz_devleri.sayi==1)
    {
        ork_birlik->mizrakcilar.savunma *=1.15;
    }
    if (arastirma_insan->savunma_ustaligi.seviye_1.sayi == 1)
    {
        insan_birlik->piyadeler.savunma *= 1.1;
        insan_birlik->okcular.savunma *= 1.1;
        insan_birlik->suvariler.savunma *= 1.1;
        insan_birlik->kusatma_makineleri.savunma *= 1.1;
    }
    if (arastirma_insan->savunma_ustaligi.seviye_2.sayi == 1)
    {
        insan_birlik->piyadeler.savunma *= 1.2;
        insan_birlik->okcular.savunma *= 1.2;
        insan_birlik->suvariler.savunma *= 1.2;
        insan_birlik->kusatma_makineleri.savunma *= 1.2;
    }

    if (arastirma_insan->savunma_ustaligi.seviye_3.sayi == 1)
    {
        insan_birlik->piyadeler.savunma *= 1.3;
        insan_birlik->okcular.savunma *= 1.3;
        insan_birlik->suvariler.savunma *= 1.3;
        insan_birlik->kusatma_makineleri.savunma *= 1.3;
    }


    if(arastirma_insan->saldiri_gelistirmesi.seviye_1.sayi == 1)
    {
        insan_birlik->piyadeler.saldiri *=1.1;
        insan_birlik->okcular.saldiri *=1.1;
        insan_birlik->suvariler.saldiri *=1.1;
        insan_birlik->kusatma_makineleri.saldiri *= 1.1;
    }
    if(arastirma_insan->saldiri_gelistirmesi.seviye_2.sayi == 1)
    {
        insan_birlik->piyadeler.saldiri *=1.2;
        insan_birlik->okcular.saldiri *=1.2;
        insan_birlik->suvariler.saldiri *=1.2;
        insan_birlik->kusatma_makineleri.saldiri *= 1.1;
    }

    if(arastirma_insan->saldiri_gelistirmesi.seviye_3.sayi == 1)
    {
        insan_birlik->piyadeler.saldiri *=1.3;
        insan_birlik->okcular.saldiri *=1.3;
        insan_birlik->suvariler.saldiri *=1.3;
        insan_birlik->kusatma_makineleri.saldiri *= 1.3;
    }

    if(arastirma_insan->elit_egitim.seviye_1.sayi == 1)
    {
        insan_birlik->piyadeler.kritik_sans += 5;
        insan_birlik->okcular.kritik_sans += 5;
        insan_birlik->suvariler.kritik_sans += 5;
        insan_birlik->kusatma_makineleri.kritik_sans += 5;
    }

    if(arastirma_insan->elit_egitim.seviye_2.sayi == 1)
    {
        insan_birlik->piyadeler.kritik_sans += 10;
        insan_birlik->okcular.kritik_sans += 10;
        insan_birlik->suvariler.kritik_sans += 10;
        insan_birlik->kusatma_makineleri.kritik_sans += 10;
    }

     if(arastirma_insan->elit_egitim.seviye_3.sayi == 1)
    {
        insan_birlik->piyadeler.kritik_sans += 15;
        insan_birlik->okcular.kritik_sans += 15;
        insan_birlik->suvariler.kritik_sans += 15;
        insan_birlik->kusatma_makineleri.kritik_sans += 15;
    }


    if(arastirma_insan->kusatma_ustaligi.seviye_1.sayi == 1)
    {
        insan_birlik->kusatma_makineleri.saldiri *= 1.1;
    }

    if(arastirma_insan->kusatma_ustaligi.seviye_2.sayi == 1)
    {
        insan_birlik->kusatma_makineleri.saldiri *= 1.2;
    }

    if(arastirma_insan->kusatma_ustaligi.seviye_3.sayi == 1)
    {
        insan_birlik->kusatma_makineleri.saldiri *= 1.3;
    }
    if (arastirma_ork->savunma_ustaligi.seviye_1.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.savunma *= 1.1;
        ork_birlik->mizrakcilar.savunma *= 1.1;
        ork_birlik->varg_binicileri.savunma *= 1.1;
        ork_birlik->troller.savunma *= 1.1;
    }
    if (arastirma_ork->savunma_ustaligi.seviye_2.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.savunma *= 1.2;
        ork_birlik->mizrakcilar.savunma *= 1.2;
        ork_birlik->varg_binicileri.savunma *= 1.2;
        ork_birlik->troller.savunma *= 1.2;
    }
    if (arastirma_ork->savunma_ustaligi.seviye_3.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.savunma *= 1.3;
        ork_birlik->mizrakcilar.savunma *= 1.3;
        ork_birlik->varg_binicileri.savunma *= 1.3;
        ork_birlik->troller.savunma *= 1.3;
    }

    if(arastirma_insan->saldiri_gelistirmesi.seviye_1.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.saldiri *=1.1;
        ork_birlik->mizrakcilar.saldiri *=1.1;
        ork_birlik->varg_binicileri.saldiri *=1.1;
        ork_birlik->troller.saldiri *= 1.1;
    }

    if(arastirma_insan->saldiri_gelistirmesi.seviye_2.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.saldiri *=1.2;
        ork_birlik->mizrakcilar.saldiri *=1.2;
        ork_birlik->varg_binicileri.saldiri *=1.2;
        ork_birlik->troller.saldiri *= 1.1;
    }

    if(arastirma_insan->saldiri_gelistirmesi.seviye_3.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.saldiri *=1.3;
        ork_birlik->mizrakcilar.saldiri *=1.3;
        ork_birlik->varg_binicileri.saldiri *=1.3;
        ork_birlik->troller.saldiri *= 1.3;
    }

    if(arastirma_ork->elit_egitim.seviye_1.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.kritik_sans += 5;
        ork_birlik->mizrakcilar.kritik_sans += 5;
        ork_birlik->varg_binicileri.kritik_sans += 5;
        ork_birlik->troller.kritik_sans += 5;
    }

    if(arastirma_ork->elit_egitim.seviye_2.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.kritik_sans += 10;
        ork_birlik->mizrakcilar.kritik_sans += 10;
        ork_birlik->varg_binicileri.kritik_sans += 10;
        ork_birlik->troller.kritik_sans += 10;
    }

    if(arastirma_ork->elit_egitim.seviye_3.sayi == 1)
    {
        ork_birlik->ork_dovusculeri.kritik_sans += 15;
        ork_birlik->mizrakcilar.kritik_sans += 15;
        ork_birlik->varg_binicileri.kritik_sans += 15;
        ork_birlik->troller.kritik_sans += 15;
    }


}


void SaglikCizimi(int x, int y, int health) {
    int barKalinlik = 40;
    int barUzunluk = 5;
    Color barRengi = (health > 50) ? (Color){0, 255, 0, 180} :
                     (health > 20) ? (Color){255, 255, 0, 180} :
                     (Color){255, 0, 0, 180};

    DrawRectangle(x - barKalinlik / 2, y - 15, barKalinlik * health / 100, barUzunluk, barRengi);
    DrawText(TextFormat("%d", health), x + 5, y - 20, 10, BLACK);
}

const char* kazananMesaj = NULL;


const char* KazananTespit(InsanImparatorlugu_Birlik *insan_birlik, OrkLegi_Birlik *ork_birlik) {


    int insanSaglik = insan_birlik->piyadeler.saglik + insan_birlik->okcular.saglik + insan_birlik->suvariler.saglik + insan_birlik->kusatma_makineleri.saglik;


    int orkSaglik = ork_birlik->ork_dovusculeri.saglik + ork_birlik->mizrakcilar.saglik + ork_birlik->varg_binicileri.saglik + ork_birlik->troller.saglik;


    if (insanSaglik <= 0 && orkSaglik > 0) {
        return "Orklar Kazandi!";
    } else if (orkSaglik <= 0 && insanSaglik > 0) {
        return "Insanlar Kazandi!";
    } else if (insanSaglik <= 0 && orkSaglik <= 0) {
        return "Berabere!";
    } else {
        return NULL;
    }
}


void savas_durum(InsanImparatorlugu_Birlik *insan_birlik, OrkLegi_Birlik *ork_birlik) {

    if (IsKeyPressed(KEY_SPACE) && kazananMesaj == NULL) {
        kazananMesaj = KazananTespit(insan_birlik, ork_birlik);
    }
}

void BirimCizme(int x, int y, Birlik birlik, Color color, const char* unitName) {
    SaglikCizimi(x, y, birlik.saglik);
    DrawCircle(x, y, 10, color);
    DrawText(unitName, x - 20, y + 15, 10, BLACK);
}


void BirimYerlestir(int startX, int startY, Birlik birlik, Color color, const char* unitName, int spacing) {
    int unitsRemaining = birlik.sayi;
    int gridX = startX;
    int gridY = startY;

    for (int i = 0; i < GRID_SIZE && unitsRemaining > 0; i++) {
        int unitsInThisCell = (unitsRemaining > MAX_UNITS_PER_CELL) ? MAX_UNITS_PER_CELL : unitsRemaining;
        int cellX = (i % GRID_SIZE) * CELL_SIZE + gridX;
        int cellY = gridY + (i / GRID_SIZE) * spacing;
        int centerX = cellX + CELL_SIZE / 2;
        int centerY = cellY + CELL_SIZE / 2;


        BirimCizme(centerX, centerY, birlik, color, unitName);

        char countText[4];
        sprintf(countText, "%d", unitsInThisCell);
        DrawText(TextFormat("Sayi:%d", unitsInThisCell), centerX - 15, centerY + 30, 10, BLACK);

        unitsRemaining -= unitsInThisCell;
    }

}


void SavasCiz(InsanImparatorlugu_Birlik *insan_birlik, OrkLegi_Birlik *ork_birlik) {
    int gridOffsetX = 50;
    int gridOffsetY = 50;

    ClearBackground(GRAY);



    for (int i = 0; i <= GRID_SIZE; i++) {
        DrawLine(gridOffsetX, gridOffsetY + i * CELL_SIZE, gridOffsetX + GRID_SIZE * CELL_SIZE, gridOffsetY + i * CELL_SIZE, LIGHTGRAY);
        DrawLine(gridOffsetX + i * CELL_SIZE, gridOffsetY, gridOffsetX + i * CELL_SIZE, gridOffsetY + GRID_SIZE * CELL_SIZE, LIGHTGRAY);
    }

    int humanSpacing = CELL_SIZE * 2;
    int orkSpacing = CELL_SIZE * 2;


    BirimYerlestir(gridOffsetX, gridOffsetY, insan_birlik->piyadeler, DARKGREEN, "Piyade", humanSpacing);
    BirimYerlestir(gridOffsetX, gridOffsetY + CELL_SIZE * 2, insan_birlik->okcular, PINK, "Okcu", humanSpacing);
    BirimYerlestir(gridOffsetX, gridOffsetY + CELL_SIZE * 4, insan_birlik->suvariler, BROWN, "Suvari", humanSpacing);
    BirimYerlestir(gridOffsetX, gridOffsetY + CELL_SIZE * 6, insan_birlik->kusatma_makineleri, BLACK , "Kusatma", humanSpacing);


    int orkStartY = gridOffsetY + CELL_SIZE * 8;

    BirimYerlestir(gridOffsetX, orkStartY + CELL_SIZE * 5 , ork_birlik->ork_dovusculeri, DARKBLUE, "OrkD", orkSpacing);
    BirimYerlestir(gridOffsetX, orkStartY + CELL_SIZE * 7, ork_birlik->mizrakcilar, RED, "Mizrakci", orkSpacing);
    BirimYerlestir(gridOffsetX, orkStartY + CELL_SIZE * 9, ork_birlik->varg_binicileri, ORANGE, "Varg", orkSpacing);
    BirimYerlestir(gridOffsetX, orkStartY + CELL_SIZE * 11, ork_birlik->troller, GREEN, "Troll", orkSpacing);


    savas_durum(insan_birlik, ork_birlik);


    if (kazananMesaj != NULL) {
        DrawText(kazananMesaj, 400, 400, 20, BLACK);
    }
}

void SavasAdimlariniHesaplama(const char* dosyayolu,InsanImparatorlugu_Birlik *insan_birlik,OrkLegi_Birlik *ork_birlik,int devir)
{
    FILE *dosya = fopen(dosyayolu , "a");
    if(dosya == NULL)
    {
        printf("Dosya acilamadi...");
        return;
    }
    fprintf(dosya, "\nTur %d:\n",devir);

    double insan_toplam_saldiri=0;
    double ork_toplam_saldiri=0;
    double insan_toplam_savunma=0;
    double ork_toplam_savunma=0;
    int tur=1;

    SaldiriGucuHesaplama(insan_birlik,ork_birlik,devir,&insan_toplam_saldiri,&ork_toplam_saldiri);
    SavunmaGucuHesaplama(insan_birlik,ork_birlik,&insan_toplam_savunma,&ork_toplam_savunma);

    if(devir%2==1)
    {
        double insan_NetHasar=insanNetHasar(insan_birlik,&insan_toplam_saldiri,&ork_toplam_savunma);
        fprintf(dosya, "Insan irki Ork irkina %f hasar verdi.\n",insan_NetHasar);
        OrkSaglikHesaplama(ork_birlik,insan_NetHasar,&ork_toplam_savunma);
        fprintf(dosya, "Ork dovusculerinin \nSayisi= %d\nSagligi=%d\n",ork_birlik->ork_dovusculeri.sayi,ork_birlik->ork_dovusculeri.saglik);
        fprintf(dosya, "Mizrakcilarin \nSayisi= %d\nSagligi=%d\n",ork_birlik->mizrakcilar.sayi,ork_birlik->mizrakcilar.saglik);
        fprintf(dosya, "Trollerin \nSayisi= %d\nSagligi=%d\n",ork_birlik->troller.sayi,ork_birlik->troller.saglik);
        fprintf(dosya, "Varg binicilerinin \nSayisi= %d\nSagligi=%d\n\n",ork_birlik->varg_binicileri.sayi,ork_birlik->varg_binicileri.saglik);
    }


    if(devir%2==0)
    {
        double ork_NetHasar=orkNetHasar(ork_birlik,&ork_toplam_saldiri,&insan_toplam_savunma);
        fprintf(dosya, "Ork irki Insan irkina %f hasar verdi.\n",ork_NetHasar);
        InsanSaglikHesaplama(insan_birlik,ork_NetHasar,&insan_toplam_savunma);
        fprintf(dosya, "Piyadelerin \nSayisi= %d\nSagligi=%d\n",insan_birlik->piyadeler.sayi,insan_birlik->piyadeler.saglik);
        fprintf(dosya, "Okcularin \nSayisi= %d\nSagligi=%d\n",insan_birlik->okcular.sayi,insan_birlik->okcular.saglik);
        fprintf(dosya, "Suvarilerin \nSayisi= %d\nSagligi=%d\n",insan_birlik->suvariler.sayi,insan_birlik->suvariler.saglik);
        fprintf(dosya, "Kusatma makinelerinin \nSayisi= %d\nSagligi=%d\n\n",insan_birlik->kusatma_makineleri.sayi,insan_birlik->kusatma_makineleri.saglik);
        devir++;
    }


    if(tur % 5 == 0)
    {
        insan_birlik->piyadeler.saldiri *= 0.9;
        insan_birlik->piyadeler.savunma *= 0.9;
        insan_birlik->okcular.saldiri *= 0.9;
        insan_birlik->okcular.savunma *= 0.9;
        insan_birlik->suvariler.saldiri *= 0.9;
        insan_birlik->suvariler.savunma *= 0.9;
        insan_birlik->kusatma_makineleri.saldiri *= 0.9;
        insan_birlik->kusatma_makineleri.savunma *= 0.9;

        ork_birlik->ork_dovusculeri.saldiri *= 0.9;
        ork_birlik->ork_dovusculeri.savunma *= 0.9;
        ork_birlik->mizrakcilar.saldiri *= 0.9;
        ork_birlik->mizrakcilar.savunma *= 0.9;
        ork_birlik->varg_binicileri.saldiri *= 0.9;
        ork_birlik->varg_binicileri.savunma *= 0.9;
        ork_birlik->troller.saldiri *= 0.9;
        ork_birlik->troller.savunma *= 0.9;
    }

    if(insan_birlik->piyadeler.sayi<=0 && insan_birlik->okcular.sayi<=0 && insan_birlik->suvariler.sayi<=0 && insan_birlik->kusatma_makineleri.sayi<=0)
    {
        fprintf(dosya, "Insan irki yok oldu! Ork legi kazandi!\n");
        return;
    }

    if(ork_birlik->ork_dovusculeri.sayi<=0 && ork_birlik->mizrakcilar.sayi<=0 && ork_birlik->troller.sayi<=0 && ork_birlik->varg_binicileri.sayi<=0)
    {
        fprintf(dosya, "Ork legi yok oldu! Insan irki kazandi!\n");
        return;
    }

    fclose(dosya);
}




int main()
{
    int devir = 1;
    int num;
    printf("1-10 arasi sayi seciniz.\n");
    scanf("%d",&num);

    DosyaIndir(num);

    jsonBirlikOkuma("unit_types.json",&insan_birlik,&ork_birlik);

    jsonKahramanOkuma("heroes.json",&insan_kahraman,&ork_kahraman);

    jsonYaratikOkuma("creatures.json",&insan_yaratik,&ork_yaratik);

    jsonArastirmaOkuma("research.json",&arastirma_insan,&arastirma_ork);

    jsonInsanSenaryosu("senaryo.json",&insan_birlik,&insan_kahraman,&insan_yaratik,&arastirma_insan);

    jsonOrkSenaryosu("senaryo.json",&ork_birlik,&ork_kahraman,&ork_yaratik,&arastirma_ork);

    BonusDeger(&insan_birlik,&ork_birlik,&insan_kahraman,&ork_kahraman,&insan_yaratik,&ork_yaratik,&arastirma_insan,&arastirma_ork);


        FILE *dosya = fopen("savas_sim.txt", "w");
        if(dosya == NULL)
        {
            printf("Dosya Acilamadi:%s\n","savas_sim.txt");
            return 0;
        }
        fclose(dosya);





    InitWindow(1000, 1000, "Savaş Simülasyonu");
    SetTargetFPS(60);


    SavasCiz(&insan_birlik, &ork_birlik);
    DrawText("Başlangıç Durumu", 50, 10, 20, BLACK);
    DrawText("Savaş başlatmak için bir tuşa basın...", 50, 30, 20, DARKGRAY);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);


        SavasCiz(&insan_birlik, &ork_birlik);

        if (IsKeyPressed(KEY_SPACE))
            {

            while (!((insan_birlik.piyadeler.sayi == 0 && insan_birlik.okcular.sayi == 0 &&
           insan_birlik.suvariler.sayi == 0 && insan_birlik.kusatma_makineleri.sayi == 0) ||
          (ork_birlik.ork_dovusculeri.sayi == 0 && ork_birlik.mizrakcilar.sayi == 0 &&
           ork_birlik.varg_binicileri.sayi == 0 && ork_birlik.troller.sayi == 0))){

    		SavasAdimlariniHesaplama("savas_sim.txt", &insan_birlik, &ork_birlik, devir);
    		devir++;
           }
            ClearBackground(RAYWHITE);
            DrawText("Savaş Sonrası Durum", 50, 10, 20, BLACK);
            SavasCiz(&insan_birlik, &ork_birlik);
            DrawText("Son durumu görmek için herhangi bir tuşa basın...", 50, 30, 20, BLACK);
            while (!WindowShouldClose() && !IsKeyPressed(KEY_SPACE)) {

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Savaş Sonrası Durum", 50, 10, 20, BLACK);
                SavasCiz(&insan_birlik, &ork_birlik);
                EndDrawing();
            }
            ClearBackground(RAYWHITE);
            DrawText("Son durum görüntülendi, çıkmak için bir tuşa basın...", 50, 30, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
