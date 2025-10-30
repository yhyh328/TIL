import java.util.*;

List<Integer> memo = new ArrayList<>();

public static int dp(int n, List<Integer> memo) {
    if (n < memo.size()) {
        return memo.get(n);
    } else {
        int value = dp(n - 1, memo) + dp(n - 2, memo);
        memo.add(value);
        return value;
    }
}

public class Hello {
    public static void main(String[] args) {

        memo.add(0);
        memo.add(1); 

        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();

        System.out.println(dp(n, memo));
    }
}