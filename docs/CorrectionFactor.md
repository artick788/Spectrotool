# Correction Factors
Given the samplelist and the Spectrofile, we need to make some corrections to the data.
This correction is based on the selected method:
- **VMM**
- **EPA**
- **Wellington**
- ...

The correction file itself is an excel table containing:
- **Compound Name**: This name is used to match the compound in the samplelist and the Spectrofile.
- **Quantified With**: This name is not relevent for the calculations but may be useful for the user to know which 
compound was used for the quantification.
- **Correction Factor**: This is the correction factor that will be applied to the data.
- **Quantity ISTD**: Not used

Since the correction file layout is the same for every method, the user should always specify what method should be used.
This gives the following function:
```
f(CompoundName, CorrectionMethod) -> CorrectionFactor, QuantifiedWith, QuantityISTD
```

## Correction for ISTD
The Spectrofile contains a IS area for each compound. The corrected IS area is calculated as:
```
CorrectedISArea = ISArea * CorrectionFactor
```
Since the Original spectrofile did not contain a correctedISArea column, add this.

## Correction for Concentration:
This correction is done in 3 steps.
1. Uncorrected Concentration:
This column is also not present in the original spectrofile. It is calculated as:
```
UncorrectedConcentration = (Area / CorrectedISArea) * ((QuantityISTD * 1000) / Weight) 
```
UncorrectedConcentration is in pg/g (Compound Weight / Sample Weight)
This info should also be presented in µg/kg (Compound Weight / Sample Weight)

2. Blank Calculation:
Some samples are blank samples, these are samples whose name begins with "BL". Take the average of these samples from the
UncorrectedConcentration column in µg/kg

So each compound would have some top information (displayed above the table) and Blank Calculation
should be displayed here.

3. Corrected Concentration (Blank Substracted Concentration):
Same as before, add new table CorrectedConcentration, calculated as:
```
CorrectedConcentration = UncorrectedConcentration - BlankAverage
```
