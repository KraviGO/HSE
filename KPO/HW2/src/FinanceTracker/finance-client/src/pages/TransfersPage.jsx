import { useEffect, useState } from "react";
import {
    Box,
    Button,
    Grid,
    MenuItem,
    TextField,
    Typography,
    Paper,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import styles from "./TransfersPage.module.css";

export default function TransfersPage() {
    const { user } = useCurrentUser();
    const [accounts, setAccounts] = useState([]);
    const [fromId, setFromId] = useState("");
    const [toId, setToId] = useState("");
    const [amount, setAmount] = useState("");

    useEffect(() => {
        if (!user) return;
        fetch(`http://localhost:5182/accounts/${user.id}`)
            .then((res) => res.json())
            .then(setAccounts)
            .catch((err) => console.error("Ошибка загрузки счетов:", err));
    }, [user]);

    const handleTransfer = async () => {
        if (!fromId || !toId || !amount) {
            alert("Заполни все поля!");
            return;
        }

        if (fromId === toId) {
            alert("Нельзя перевести на тот же счёт!");
            return;
        }

        try {
            const res = await fetch("http://localhost:5182/transfer", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    userId: user.id,
                    fromId,
                    toId,
                    amount: parseFloat(amount),
                }),
            });

            if (res.ok) {
                alert("Перевод выполнен успешно!");
                setAmount("");
                const updated = await fetch(`http://localhost:5182/accounts/${user.id}`).then((r) =>
                    r.json()
                );
                setAccounts(updated);
            } else {
                const text = await res.text();
                alert("Ошибка перевода: " + text);
            }
        } catch (err) {
            console.error("Ошибка при переводе:", err);
            alert("Ошибка соединения с сервером!");
        }
    };

    return (
        <Box className={styles.container}>
            <Paper elevation={2} className={styles.card}>
                <Typography variant="h5" className={styles.title}>
                    🔁 Перевод между счетами
                </Typography>

                {!user ? (
                    <Typography color="error" align="center">
                        Сначала выбери пользователя во вкладке «Пользователи».
                    </Typography>
                ) : (
                    <Grid container spacing={2} justifyContent="center" alignItems="center">
                        <Grid item xs={12} md={4}>
                            <TextField
                                select
                                label="Счёт-источник"
                                value={fromId}
                                onChange={(e) => setFromId(e.target.value)}
                                fullWidth
                                className={styles.select}
                            >
                                {accounts.map((acc) => (
                                    <MenuItem key={acc.id} value={acc.id}>
                                        {acc.name} — {acc.balance} {acc.currency}
                                    </MenuItem>
                                ))}
                            </TextField>
                        </Grid>

                        <Grid item xs={12} md={4}>
                            <TextField
                                select
                                label="Счёт-получатель"
                                value={toId}
                                onChange={(e) => setToId(e.target.value)}
                                fullWidth
                                className={styles.select}
                            >
                                {accounts.map((acc) => (
                                    <MenuItem key={acc.id} value={acc.id}>
                                        {acc.name} — {acc.balance} {acc.currency}
                                    </MenuItem>
                                ))}
                            </TextField>
                        </Grid>

                        <Grid item xs={12} md={2}>
                            <TextField
                                label="Сумма"
                                type="number"
                                value={amount}
                                onChange={(e) => setAmount(e.target.value)}
                                fullWidth
                            />
                        </Grid>

                        <Grid item xs={12} md={2}>
                            <Button variant="contained" onClick={handleTransfer} className={styles.button}>
                                Перевести
                            </Button>
                        </Grid>
                    </Grid>
                )}
            </Paper>
        </Box>
    );
}