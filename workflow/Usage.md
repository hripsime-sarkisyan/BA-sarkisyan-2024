# General
Before running your clustering experiment you have to populate the graph database using the `SettlementDelineationNeighbours` executable from the **SDA-Workflow**. For convenience, a wrapper python script executes the binary in the container and keeps the graph database running after the settlement graph was constructed. Additionally, a frontend to the database is available with [MemgraphLab](http://localhost:3000). Since the Script does not stop the containers, the database must be stopped manually via the Docker CLI or Docker Desktop. 
# Graph Construction
To create the settlement graph in the database run the following command:
```
python3 SDAGraphConstruction.py -i <Filtered-Input.shp> -c graph-construction-config.json 
```
For example: 
```
python3 SDAGraphConstruction.py -i ../resources/sda-workflow/Corvara_IT_filtered.shp -c graph-construction-config.json 
```
# Clearing Memgraph
If you want to clear the graph database, you can do it programmatically in the code via `Graph::clear()` or using the [MemgraphLab](http://localhost:3000) and running `MATCH (n) DETACH DELETE n;` in the *Query Execution Console*