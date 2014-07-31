
void
sqlite_table_print(struct Table* T) {
        DBG("new table <%s> column-%d ", T->zName, T->nCol);
}

void sqlite_db_print(sqlite3 *db) {
        int i;
        // pVfs
        if(db->pVfs) {
                sqlite3_vfs *V = db->pVfs;
                do {
                        DBG("->pVfs (V=%d, S=%d, n=%s)", V->iVersion, V->szOsFile, V->zName);
                        V=V->pNext;
                }while(V);
        }
        //Vdbe
//      if(db->pVdbe)
//              DBG("->Vdbe (%s)", db->pVdbe->zSql);
        //Db
        DBG("->nDb Numbers of backends (%d)", db->nDb);
        for(i=0; i<db->nDb; i++) {
                DBG("->nDb *%d*", i);
                DBG("       %s", db->aDb[i].zName);
                //Btree
                DBG("       Btree");
                struct Btree* V = db->aDb[i].pBt;
                DBG("          %x", V);
//              do {
//                      DBG("          %x", V);
//                      V = V->pNext;
//              } while(V);
                //Schema
                DBG("       SCHEMA (COO %d, GEN %d, SIZE %d)", db->aDb[i].pSchema->schema_cookie, db->aDb[i].pSchema->iGeneration, db->aDb[i].pSchema->cache_size);
        }
        //...
        DBG("->activeVdbeCnt %d", db->activeVdbeCnt);
        DBG("->writeVdbeCnt %d", db->writeVdbeCnt);
        DBG("->vdbeExecCnt %d", db->vdbeExecCnt);
        DBG("->nExtension %d", db->nExtension);
}

