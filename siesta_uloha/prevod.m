ntc_data = csvread("ntc.csv");
t = ntc_data(:, 1);
resistance = ntc_data(:, 2);


adc = (resistance ./ (resistance + 10)) * 1023;
plot(adc, t, 'o')

p = polyfit(adc, t, 10);

ad2 = 0:1023;
t2 = round(polyval(p, ad2), 1);
hold on, plot(ad2, t2, 'red');

dlmwrite('data.dlm', t2*10, ',');